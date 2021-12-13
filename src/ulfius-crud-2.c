/**
 * Author: Nicolas Mora <mail@babelouest.io>
 * License: MIT
 */
#include <stdio.h>
#include <jansson.h>
#include <ulfius.h>
#include <hoel.h>

#define PORT 8080

int callback_get_book_list(const struct _u_request * request, struct _u_response * response, void * user_data) {
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
  res = h_select((struct _h_connection *)user_data, j_query, &j_result, NULL);
  // Deallocate j_query since it won't be needed anymore
  json_decref(j_query);
  // Test query execution result
  if (res == H_OK) {
    // j_result contains all our data in a JSON array of JSON objects
    ulfius_set_json_body_response(response, 200, j_result);
    json_decref(j_result);
  } else {
    response->status = 500;
    fprintf(stderr, "Error executing select query\n");
  }
  return U_CALLBACK_CONTINUE;
}

int callback_get_book(const struct _u_request * request, struct _u_response * response, void * user_data) {
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
  res = h_select((struct _h_connection *)user_data, j_query, &j_result, NULL);
  // Deallocate j_query since it won't be needed anymore
  json_decref(j_query);
  // Test query execution result
  if (res == H_OK) {
    // if j_result is empty, send an error 404
    // otherwise send the first element inside j_result
    if (json_array_size(j_result) > 0) {
      ulfius_set_json_body_response(response, 200, json_array_get(j_result, 0));
    } else {
      response->status = 404;
    }
    json_decref(j_result);
  } else {
    response->status = 500;
    fprintf(stderr, "Error executing select query\n");
  }
  return U_CALLBACK_CONTINUE;
}

int callback_add_book(const struct _u_request * request, struct _u_response * response, void * user_data) {
  json_t * j_book = ulfius_get_json_body_request(request, NULL), * j_query;
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
  res = h_insert((struct _h_connection *)user_data, j_query, NULL);
  // Deallocate j_query since it won't be needed anymore
  json_decref(j_query);
  // Test query execution result
  if (res != H_OK) {
    response->status = 500;
    fprintf(stderr, "Error executing insert query\n");
  }
  // Default response status with ulfius is 200, so only the error insert must be handled

  json_decref(j_book);
  return U_CALLBACK_CONTINUE;
}

int callback_set_book(const struct _u_request * request, struct _u_response * response, void * user_data) {
  json_t * j_query, * j_result = NULL, * j_book = ulfius_get_json_body_request(request, NULL);
  int res;

  // First, look for the isbn in the url, if not found, do not update but send an error 404
  j_query = json_pack("{sss[s]s{ss}}",
                      "table", "books",
                      "columns",
                        "id",
                      "where",
                        "isbn", u_map_get(request->map_url, "isbn"));

  // Execute the query
  res = h_select((struct _h_connection *)user_data, j_query, &j_result, NULL);
  // Deallocate j_query since it won't be needed anymore
  json_decref(j_query);
  // Test query execution result
  if (res == H_OK) {
    if (json_array_size(j_result) > 0) {
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
      res = h_update((struct _h_connection *)user_data, j_query, NULL);
      // Deallocate j_query since it won't be needed anymore
      json_decref(j_query);
      // Test query execution result
      if (res != H_OK) {
        response->status = 500;
        fprintf(stderr, "Error executing insert query\n");
      }
    } else {
      response->status = 404;
    }
    json_decref(j_result);
  } else {
    response->status = 500;
    fprintf(stderr, "Error executing select query\n");
  }

  json_decref(j_book);
  return U_CALLBACK_CONTINUE;
}

int callback_delete_book(const struct _u_request * request, struct _u_response * response, void * user_data) {
  json_t * j_query, * j_result = NULL, * j_body = ulfius_get_json_body_request(request, NULL);
  int res;

  // First, look for the isbn in the url, if not found, do not update but send an error 404
  j_query = json_pack("{sss[s]s{ss}}",
                      "table", "books",
                      "columns",
                        "id",
                      "where",
                        "isbn", u_map_get(request->map_url, "isbn"));

  // Execute the query
  res = h_select((struct _h_connection *)user_data, j_query, &j_result, NULL);
  // Deallocate j_query since it won't be needed anymore
  json_decref(j_query);
  // Test query execution result
  if (res == H_OK) {
    if (json_array_size(j_result) > 0) {
      // The book values are from the HTTP request, hoel will automatically sanitize them
      // The isbn value is from the HTTP query string, hoel will automatically sanitize it
      // If json_string_value() returns NULL, the value will be ignored because of the format ss*
      j_query = json_pack("{sss{ss}}",
                          "table", "books",
                          "where",
                            "isbn", u_map_get(request->map_url, "isbn"));

      // Execute the query
      res = h_delete((struct _h_connection *)user_data, j_query, NULL);
      // Deallocate j_query since it won't be needed anymore
      json_decref(j_query);
      // Test query execution result
      if (res == H_OK) {
        // Update OK
      } else {
        fprintf(stderr, "Error executing insert query\n");
      }
    } else {
      response->status = 404;
    }
    json_decref(j_result);
  } else {
    response->status = 500;
    fprintf(stderr, "Error executing select query\n");
  }

  json_decref(j_body);
  return U_CALLBACK_CONTINUE;
}

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
