---
layout: post
title:  "Glewlwyd tutorial - The code authentication flow"
date:   Sat, 16 Apr 2022 14:43:45 -0400
tags: glewlwyd flow
---

OAuth2 framework describes multiple flows for client applications to authenticate users against an Authentication Server (AS), so the client applications can access protected resources owned by the user. These flows allows secure access, and forbid client applications to get more access than what's necessary, without allowing the client to impersonate the user.

You can test this flow on [esras](https://esras.babelouest.io/) using a client with the `authorization_code` grant type allowed.

![esras-registration-1](/screenshots/esras-registration-1.png)

The first authentication flow is the `code` grant type.

The client starts the flow by redirecting the user to the AS `/auth` endpoint, here is an example of an `/auth` url built by the client:

`https://glewlwyd.babelouest.io/api/oidc/auth?redirect_uri=https%3A%2F%2Fesras.babelouest.io%2Fapi%2Fexec%2Fcallback&response_type=code&client_id=68fklr3bol3s8phc&state=5lEGRXVxdHITK19y&scope=openid+mfa-1&nonce=WYwRUtiMr2Dj77sSSzeuIVyTljDYkEJT`

The `/auth` parameters are:

- `https://glewlwyd.babelouest.io/api/oidc/auth`: the url to the `/auth` endpoint
- `redirect_uri`: the redirect uri is an uri linked to the client, the user will be redirected to this uri if the authentication is succesfull, mandatory
- `response_type` the response type specifies the flow to use, in this case the response type is `code`, mandatory
- `client_id`: the client identifier, this id is used by the AS to identify the client, mandatory
- `state`: a state is a unique value used by the client to identify the request, optional but necessary in some cases
- `scope`: the scope is a list of scopes required by the client for the flow, optional in the OAuth2 specification, but usually required by the AS
- `nonce` : a nonce value is a random value, with high entropy, mandatory for OpenID Connect flows

