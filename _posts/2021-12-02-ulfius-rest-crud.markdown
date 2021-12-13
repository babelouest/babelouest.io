---
layout: post
title:  "Ulfius REST CRUD tutorial #1 - A simple CRUD API"
date:   Thu, 02 Dec 2021 19:17:05 -0500
tags: ulfius tutorial
order: 2
---

This is the first chapter of the tutorial: `Build a REST CRUD API`.

Let's say your client needs you to build a REST API to provide a CRUD for books, the client will take care of the front-end.
Although you need to implement a backend API that will match the following pattern:

- `GET` `/api/book`              => get a list of books
- `GET` `/api/book/{some_id}`    => get a specific book
- `POST` `/api/book`             => add a new book
- `PUT` `/api/book/{some_id}`    => update an existing book
- `DELETE` `/api/book/{some_id}` => delete an existing book

A book is a JSON object with the following model:

{% highlight JSON %}
{
  "ISBN": "9780131101630",
  "title": "The C Programming Language",
  "author": "Brian Kernighan, Dennis Ritchie",
  "date": "1978"
}
{% endhighlight %}

The front-end and the backend applications will run on different urls. The backend will run on the port `8080`.

### The instance

This is the simpliest way to declare the instance listenting on port `8080`

{% highlight C %}
struct _u_instance instance;

ulfius_init_instance(&instance, 8080, NULL, NULL);
{% endhighlight %}

It's recommended to test the return value of `ulfius_init_instance`, as well as any other function call that return the result in `int` format.

### The endpoints

The endpoints are declared like this:

{% highlight C %}
ulfius_add_endpoint_by_val(&instance, "GET", "/api", "/book/", 1, &callback_get_book_list, shared_data);
ulfius_add_endpoint_by_val(&instance, "GET", "/api", "/book/:isbn", 1, &callback_get_book, shared_data);
ulfius_add_endpoint_by_val(&instance, "POST", "/api", "/book/", 1, &callback_add_book, shared_data);
ulfius_add_endpoint_by_val(&instance, "PUT", "/api", "/book/:isbn", 1, &callback_set_book, shared_data);
ulfius_add_endpoint_by_val(&instance, "DELETE", "/api", "/book/:isbn", 1, &callback_delete_book, shared_data);
{% endhighlight %}

### The callback functions

The callback functions will share a JSON array that will store the book list. Here we will use [Jansson](https://github.com/akheron/jansson) library.

{% highlight C %}
json_t * shared_data  = json_array();
{% endhighlight %}

The `callback_get_book_list` will simply return the content of shared_data:

{% highlight C %}
int callback_get_book_list(const struct _u_request * request, struct _u_response * response, void * user_data) {
  ulfius_set_json_body_response(response, 200, (json_t *)user_data);
  return U_CALLBACK_CONTINUE;
}
{% endhighlight %}

The `callback_get_book` will look for the ISBN in the book list, then return it, or send an error 404 if not found:

{% highlight C %}
int callback_get_book(const struct _u_request * request, struct _u_response * response, void * user_data) {
  json_t * j_book;
  size_t index;
  int found = 0;
  
  json_array_foreach((json_t *)user_data, index, j_book) {
    if (0 == o_strcmp(u_map_get(request->map_url, "isbn"), json_string_value(json_object_get(j_book, "ISBN")))) {
      ulfius_set_json_body_response(response, 200, j_book);
      found = 1;
    }
  }
  if (!found) {
    response->status = 404;
  }
  return U_CALLBACK_CONTINUE;
}
{% endhighlight %}

The `callback_add_book` will add the book from the request body to the book list

{% highlight C %}
int callback_add_book(const struct _u_request * request, struct _u_response * response, void * user_data) {
  json_t * j_book = ulfius_get_json_body_request(request, NULL);
  json_array_append((json_t *)user_data, j_book);
  json_decref(j_book);
  return U_CALLBACK_CONTINUE;
}
{% endhighlight %}

The `callback_set_book` will look for the ISBN in the book list, then update it, or send an error 404 if not found:

{% highlight C %}
int callback_set_book(const struct _u_request * request, struct _u_response * response, void * user_data) {
  json_t * j_book, * j_body = ulfius_get_json_body_request(request, NULL);
  size_t index;
  int found = 0;
  
  json_array_foreach((json_t *)user_data, index, j_book) {
    if (0 == o_strcmp(u_map_get(request->map_url, "isbn"), json_string_value(json_object_get(j_book, "ISBN")))) {
      json_array_set((json_t *)user_data, index, j_body);
      found = 1;
    }
  }
  if (!found) {
    response->status = 404;
  }
  json_decref(j_body);
  return U_CALLBACK_CONTINUE;
}
{% endhighlight %}

And finally, the `callback_delete_book` will look for the ISBN in the book list, then remove it, or send an error 404 if not found:

{% highlight C %}
int callback_delete_book(const struct _u_request * request, struct _u_response * response, void * user_data) {
  json_t * j_book;
  size_t index;
  int found = 0;
  
  json_array_foreach((json_t *)user_data, index, j_book) {
    if (0 == o_strcmp(u_map_get(request->map_url, "isbn"), json_string_value(json_object_get(j_book, "ISBN")))) {
      json_array_remove((json_t *)user_data, index);
      found = 1;
    }
  }
  if (!found) {
    response->status = 404;
  }
  return U_CALLBACK_CONTINUE;
}
{% endhighlight %}

### Download source code

The source code for this program can be found at this address: [/src/ulfius-crud-1.c](/src/ulfius-crud-1.c).

To compile this program, run `gcc -o ulfius-crud-1 ulfius-crud-1.c -ljansson -lorcania -lulfius`.
