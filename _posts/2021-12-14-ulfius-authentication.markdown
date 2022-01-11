---
layout: post
title:  "Ulfius REST CRUD tutorial #3 - Add authentication"
date:   Tue, 14 Dec 2021 13:11:30 -0500
tags: ulfius tutorial
order: 4
---

This is the third chapter of the tutorial: `Build a REST CRUD API`.

Last time, we added a database backend for the persistence. Now let's add authentication, so users who want to access this API must be verified.

### Authentication endpoint

In Ulfius framework, since you can have multiple endpoints for the same address, add an endpoint for all `/api` calls with a higher priority than the data management callbacks.

{% highlight C %}
ulfius_add_endpoint_by_val(&instance, "*", "/api/", "*", 0, &callback_auth, shared_data); priority is set to 0, first callback to call
ulfius_add_endpoint_by_val(&instance, "GET", "/api", "/book/", 1, &callback_get_book_list, shared_data); priority is set to 1, second callback to call
ulfius_add_endpoint_by_val(&instance, "GET", "/api", "/book/:isbn", 1, &callback_get_book, shared_data);
ulfius_add_endpoint_by_val(&instance, "POST", "/api", "/book/", 1, &callback_add_book, shared_data);
ulfius_add_endpoint_by_val(&instance, "PUT", "/api", "/book/:isbn", 1, &callback_set_book, shared_data);
ulfius_add_endpoint_by_val(&instance, "DELETE", "/api", "/book/:isbn", 1, &callback_delete_book, shared_data);
{% endhighlight %}

When you chain callback functions, if a cllback function returns another value than `U_CALLBACK_CONTINUE`, the chain stops and the other callback functions are ignored. Therefore you can easily separate purposes in different callback levels.

### HTTP Basic access authentication

A simple authentication method is the [Basic access authentication](https://en.wikipedia.org/wiki/Basic_access_authentication). In Ulfius, the user and the password send by the client using basic auth are accessible in the `struct _u_request` elements `auth_basic_user` and `auth_basic_password`:

{% highlight C %}
// Basic access authentication callback function
int callback_auth(const struct _u_request * request, struct _u_response * response, void * user_data) {
  const char user[] = "dave", password[] = "password";
  
  // o_strcmp is the orcania version of strcmp, except it doesn't segfault if one of the argument is NULL
  if (0 == o_strcmp(user, request->auth_basic_user) && 0 == o_strcmp(password, request->auth_basic_password)) {
    // User is authenticated
    return U_CALLBACK_CONTINUE;
  } else {
    // User isn't authenticated, the next callback will not be executed
    return U_CALLBACK_UNAUTHORIZED;
  }
}
{% endhighlight %}

### Authorization header

When the authentication comes from an `Authorization` header, the header value must be verified itself. For example, if `Authorization` header requires to contain a specified value, the verification can be as follow:

{% highlight C %}
// Basic access authentication callback function
int callback_auth(const struct _u_request * request, struct _u_response * response, void * user_data) {
  const char token[] = "myTokenXyz1234";
  
  // o_strcmp is the orcania version of strcmp, except it doesn't segfault if one of the argument is NULL
  if (0 == o_strcmp(token, u_map_get(request->map_header, "Authorization"))) {
    // Request is authenticated
    return U_CALLBACK_CONTINUE;
  } else {
    // Request isn't authenticated, the next callback will not be executed
    return U_CALLBACK_UNAUTHORIZED;
  }
}
{% endhighlight %}

You can find more examples of header verification in the [example callbacks](https://github.com/babelouest/ulfius/tree/master/example_callbacks) folder in Ulfius' GitHub page.
