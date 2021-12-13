---
layout: post
title:  "Ulfius REST CRUD tutorial #2 - Add database backend"
date:   Sun, 12 Dec 2021 11:29:10 -0500
tags: ulfius tutorial
order: 3
---

This is the second chapter of the tutorial: `Build a REST CRUD API`.

In the first chapter, we saw how to build a simple REST API with Ulfius. Now imagine the client likes the backend and how you built it, but having no persistence is a problem, since the client has to re-enter all the books, every time the program restarts.

Therefore you'll want to add a persistence backend to save and retrieve your data. We'll use the library [Hoel](https://github.com/babelouest/hoel) for this, because it's a database backend library that abstract most of the work needed. This library works with SQLite, Mysql/MariaDB and PostgreSQL databases. Here we'll use a SQLite database file to store data.

### The database table

Here is the SQL query to create the table that will store our books:

{% highlight JSON %}
CREATE TABLE books (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    isbn TEXT,
    title TEXT,
    author TEXT,
    year TEXT
);
{% endhighlight %}

The database file will be saved in `/tmp/books.db`

{% highlight shell %}
$ sqlite3 /tmp/books.db
SQLite version 3.34.1 2021-01-20 14:10:07
Enter ".help" for usage hints.
sqlite> CREATE TABLE books (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    isbn TEXT,
    title TEXT,
    author TEXT,
    year TEXT
);
{% endhighlight %}

### Hoel library initialization

Hoel uses a `struct _h_connection` to store the connection instance, and uses JSON data to build queries and get results. It requires [Jansson](http://www.digip.org/jansson/) library.

The core functions we'll use are the following:

- `h_connect_sqlite` to open the database file and initialize the instance
- `h_close_db` to close the database file
- `h_clean_connection` to cleanup the database instance

These functions will be executed in the `main` function. So our main function will now look like this:

{% highlight C %}
int main(void) {
  struct _u_instance instance;
  struct _h_connection * conn = h_connect_sqlite("/tmp/books.db");

  // Initialize instance with the port number
  if (ulfius_init_instance(&instance, PORT, NULL, NULL) != U_OK) {
    fprintf(stderr, "Error ulfius_init_instance, abort\n");
    return(1);
  }

  // Endpoint list declaration
  ulfius_add_endpoint_by_val(&instance, "GET", "/api", "/book/", 1, &callback_get_book_list, conn);
  ulfius_add_endpoint_by_val(&instance, "GET", "/api", "/book/:isbn", 1, &callback_get_book, conn);
  ulfius_add_endpoint_by_val(&instance, "POST", "/api", "/book/", 1, &callback_add_book, conn);
  ulfius_add_endpoint_by_val(&instance, "PUT", "/api", "/book/:isbn", 1, &callback_set_book, conn);
  ulfius_add_endpoint_by_val(&instance, "DELETE", "/api", "/book/:isbn", 1, &callback_delete_book, conn);

  // Start the framework
  if (ulfius_start_framework(&instance) == U_OK) {
    printf("Start framework on port %d\n", instance.port);

    // Wait for the user to press <enter> on the console to quit the application
    getchar();
  } else {
    fprintf(stderr, "Error starting framework\n");
  }
  printf("End framework\n");

  h_close_db(conn);
  h_clean_connection(conn);
  ulfius_stop_framework(&instance);
  ulfius_clean_instance(&instance);

  return 0;
}
{% endhighlight %}

So instead of passing the variable `shared_data` in the callback functions, the database connection will be used instead.

### Select queries

With Hoel, a select query will use the `h_select` function:

{% highlight C %}
/**
 * h_select
 * Execute a select query
 * Uses a json_t * parameter for the query parameters
 * Store the result of the query in j_result if specified. j_result must be decref'd after use
 * Duplicate the generated query in generated_query if specified, must be h_free'd after use
 * return H_OK on success
 */
int h_select(const struct _h_connection * conn, const json_t * j_query, json_t ** j_result, char ** generated_query);
{% endhighlight %}

In our example, to select all the books, you will use `h_select` like this:

{% highlight C %}
json_t * j_query, * j_result = NULL;
int res;

j_query = json_pack("{sss[ssss]}",
                    "table", "books",
                    "columns",
                      "isbn AS ISBN",
                      "title",
                      "author",
                      "year");

// Execute the query
res = h_select(conn, j_query, &j_result, NULL);
// Deallocate j_query since it won't be needed anymore
json_decref(j_query);
// Test query execution result
if (res == H_OK) {
  // j_result contains all our data in a JSON array of JSON objects
  json_decref(j_result);
} else {
  fprintf(stderr, "Error executing select query\n");
}
{% endhighlight %}

You'll need to use this code inside your `callback_get_book_list`.

To use this with the `callback_get_book`, simply add a `where` clause.

{% highlight C %}
json_t * j_query, * j_result = NULL;
int res;

// The isbn value is from the HTTP query string, hoel will automatically sanitize it
j_query = json_pack("{sss[ssss]s{ss}}",
                    "table", "books",
                    "columns",
                      "isbn AS ISBN",
                      "title",
                      "author",
                      "year",
                    "where",
                      "isbn", u_map_get(request->map_url, "isbn"));

// Execute the query
res = h_select(conn, j_query, &j_result, NULL);
// Deallocate j_query since it won't be needed anymore
json_decref(j_query);
// Test query execution result
if (res == H_OK) {
  // if j_result is empty, send an error 404
  // otherwise send the first element inside j_result
  json_decref(j_result);
} else {
  fprintf(stderr, "Error executing select query\n");
}
{% endhighlight %}

### Insert queries

The insert query will use `h_insert` function:

{% highlight C %}
/**
 * h_insert
 * Execute an insert query
 * Uses a json_t * parameter for the query parameters
 * Duplicate the generated query in generated_query if specified, must be h_free'd after use
 * return H_OK on success
 */
int h_insert(const struct _h_connection * conn, const json_t * j_query, char ** generated_query);
{% endhighlight %}

So to add a new book, your code will look like this:

{% highlight C %}
json_t * j_query;
int res;

// The values are from the HTTP request, hoel will automatically sanitize them
// If json_string_value() returns NULL, the value will be ignored because of the format ss*
j_query = json_pack("{sss{ss*ss*ss*ss*}}",
                    "table", "books",
                    "values",
                      "isbn", json_string_value(json_object_get(j_book, "ISBN")),
                      "title", json_string_value(json_object_get(j_book, "title")),
                      "author", json_string_value(json_object_get(j_book, "author")),
                      "year",  json_string_value(json_object_get(j_book, "year")));

// Execute the query
res = h_insert(conn, j_query, NULL);
// Deallocate j_query since it won't be needed anymore
json_decref(j_query);
// Test query execution result
if (res == H_OK) {
  // Insert OK
} else {
  fprintf(stderr, "Error executing insert query\n");
}
{% endhighlight %}

### Update queries

The update query will use `h_update` function:

{% highlight C %}
/**
 * h_update
 * Execute an update query
 * Uses a json_t * parameter for the query parameters
 * Duplicate the generated query in generated_query if specified, must be h_free'd after use
 * return H_OK on success
 */
int h_update(const struct _h_connection * conn, const json_t * j_query, char ** generated_query);
{% endhighlight %}

So to update a book, your code will look like this:

{% highlight C %}
json_t * j_query;
int res;

// The book values are from the HTTP request, hoel will automatically sanitize them
// The isbn value is from the HTTP query string, hoel will automatically sanitize it
// If json_string_value() returns NULL, the value will be ignored because of the format ss*
j_query = json_pack("{sss{ss*ss*ss*}s{ss*}}",
                    "table", "books",
                    "set",
                      "title", json_string_value(json_object_get(j_book, "title")),
                      "author", json_string_value(json_object_get(j_book, "author")),
                      "year",  json_string_value(json_object_get(j_book, "year")),
                    "where",
                      "isbn", u_map_get(request->map_url, "isbn"));

// Execute the query
res = h_update(conn, j_query, NULL);
// Deallocate j_query since it won't be needed anymore
json_decref(j_query);
// Test query execution result
if (res == H_OK) {
  // Update OK
} else {
  fprintf(stderr, "Error executing insert query\n");
}
{% endhighlight %}

### Delete queries

The delete query will use `h_delete` function:

{% highlight C %}
/**
 * h_delete
 * Execute a delete query
 * Uses a json_t * parameter for the query parameters
 * Duplicate the generated query in generated_query if specified, must be h_free'd after use
 * return H_OK on success
 */
int h_delete(const struct _h_connection * conn, const json_t * j_query, char ** generated_query);
{% endhighlight %}

So to delete a book, your code will look like this:

{% highlight C %}
json_t * j_query;
int res;

// The isbn value is from the HTTP query string, hoel will automatically sanitize it
// If json_string_value() returns NULL, the value will be ignored because of the format ss*
if (u_map_get(request->map_url, "isbn") != NULL) {
  j_query = json_pack("{sss{ss}}",
                      "table", "books",
                      "where",
                        "isbn", u_map_get(request->map_url, "isbn"));

  // Execute the query
  res = h_delete(conn, j_query, NULL);
  // Deallocate j_query since it won't be needed anymore
  json_decref(j_query);
  // Test query execution result
  if (res == H_OK) {
    // Update OK
  } else {
    fprintf(stderr, "Error executing insert query\n");
  }
}
{% endhighlight %}

### Download source code

The source code for this program can be found at this address: [/src/ulfius-crud-2.c](/src/ulfius-crud-2.c).

To compile this program, run `gcc -o ulfius-crud-2 ulfius-crud-2.c -ljansson -lorcania -lulfius -lhoel`.
