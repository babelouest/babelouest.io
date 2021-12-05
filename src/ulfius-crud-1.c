/**
 * Author: Nicolas Mora <info@babelouest.io>
 * License: MIT
 */
#include <stdio.h>
#include <jansson.h>
#include <ulfius.h>

#define PORT 8080

int callback_get_book_list(const struct _u_request * request, struct _u_response * response, void * user_data) {
  ulfius_set_json_body_response(response, 200, (json_t *)user_data);
  return U_CALLBACK_CONTINUE;
}

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

int callback_add_book(const struct _u_request * request, struct _u_response * response, void * user_data) {
  json_t * j_book = ulfius_get_json_body_request(request, NULL);
  json_array_append((json_t *)user_data, j_book);
  json_decref(j_book);
  return U_CALLBACK_CONTINUE;
}

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

int main(void) {
  struct _u_instance instance;
  json_t * shared_data  = json_array();

  // Initialize instance with the port number
  if (ulfius_init_instance(&instance, PORT, NULL, NULL) != U_OK) {
    fprintf(stderr, "Error ulfius_init_instance, abort\n");
    return(1);
  }

  // Endpoint list declaration
  ulfius_add_endpoint_by_val(&instance, "GET", "/api", "/book/", 1, &callback_get_book_list, shared_data);
  ulfius_add_endpoint_by_val(&instance, "GET", "/api", "/book/:isbn", 1, &callback_get_book, shared_data);
  ulfius_add_endpoint_by_val(&instance, "POST", "/api", "/book/", 1, &callback_add_book, shared_data);
  ulfius_add_endpoint_by_val(&instance, "PUT", "/api", "/book/:isbn", 1, &callback_set_book, shared_data);
  ulfius_add_endpoint_by_val(&instance, "DELETE", "/api", "/book/:isbn", 1, &callback_delete_book, shared_data);

  // Start the framework
  if (ulfius_start_framework(&instance) == U_OK) {
    printf("Start framework on port %d\n", instance.port);

    // Wait for the user to press <enter> on the console to quit the application
    getchar();
  } else {
    fprintf(stderr, "Error starting framework\n");
  }
  printf("End framework\n");

  json_decref(shared_data);
  ulfius_stop_framework(&instance);
  ulfius_clean_instance(&instance);

  return 0;
}
