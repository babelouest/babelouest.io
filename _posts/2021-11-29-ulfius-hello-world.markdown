---
layout: post
title:  "Ulfius - A simple Hello World!"
date:   Mon, 29 Nov 2021 18:30:04 -0500
tags: ulfius
order: 1
---

The following program is a webservice sending `Hello World!` to the clients, written with Ulfius.

{% highlight C %}
#include <stdio.h>
#include <ulfius.h>

#define PORT 8080

/**
 * Callback function for the web application on /helloworld url call
 */
int callback_hello_world (const struct _u_request * request, struct _u_response * response, void * user_data) {
  ulfius_set_string_body_response(response, 200, "Hello World!");
  return U_CALLBACK_CONTINUE;
}

/**
 * main function
 */
int main(void) {
  struct _u_instance instance;

  // Initialize instance with the port number
  if (ulfius_init_instance(&instance, PORT, NULL, NULL) != U_OK) {
    fprintf(stderr, "Error ulfius_init_instance, abort\n");
    return(1);
  }

  // Endpoint list declaration
  ulfius_add_endpoint_by_val(&instance, "GET", "/helloworld", NULL, 0, &callback_hello_world, NULL);

  // Start the framework
  if (ulfius_start_framework(&instance) == U_OK) {
    printf("Start framework on port %d\n", instance.port);

    // Wait for the user to press <enter> on the console to quit the application
    getchar();
  } else {
    fprintf(stderr, "Error starting framework\n");
  }
  printf("End framework\n");

  ulfius_stop_framework(&instance);
  ulfius_clean_instance(&instance);

  return 0;
}
{% endhighlight %}

This program initializes an Ulfius instance on port 8080:

{% highlight C %}
#define PORT 8080
struct _u_instance instance;
if (ulfius_init_instance(&instance, PORT, NULL, NULL) != U_OK) {
{% endhighlight %}

Then it declares the endpoint `GET` `/helloworld` with the callback function `callback_hello_world`

{% highlight C %}
ulfius_add_endpoint_by_val(&instance, "GET", "/helloworld", NULL, 0, &callback_hello_world, NULL);
{% endhighlight %}

The callback function in this case simply sends the HTTP status 200 and the string `Hello World!` as HTTP body response. Another tutorial document will the callback function with more details.

{% highlight C %}
int callback_hello_world (const struct _u_request * request, struct _u_response * response, void * user_data) {
  ulfius_set_string_body_response(response, 200, "Hello World!");
  return U_CALLBACK_CONTINUE;
}
{% endhighlight %}

After that, the instance is started, which means that the program will listen to HTTP connexions on the specified port.

{% highlight C %}
if (ulfius_start_framework(&instance) == U_OK) {
  getchar();
{% endhighlight %}

The webservice is executed in a separate thread, so the  function `ulfius_start_framework` will return imediately. The call to `getchar()` after is used for the program to wait for the user to press `<enter>` before closing the program.

Before closing the program, the instance must be stopped and cleaned.

{% highlight C %}
ulfius_stop_framework(&instance);
ulfius_clean_instance(&instance);
{% endhighlight %}
