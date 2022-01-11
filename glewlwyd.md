---
layout: default
title: Glewlwyd
permalink: /glewlwyd/
---

# Glewlwyd

Glewlwyd is a Single Sign-on server with multiple factor authentication for OAuth2 and OpenID Connect authentication.

[![GitHub release (latest by date)](https://img.shields.io/github/v/release/babelouest/glewlwyd?style=plastic)](https://github.com/babelouest/glewlwyd/releases/latest)
[![GitHub last commit](https://img.shields.io/github/last-commit/babelouest/glewlwyd)](https://github.com/babelouest/glewlwyd/)
![GitHub](https://img.shields.io/github/license/babelouest/glewlwyd?style=plastic)
![CodeQL](https://github.com/babelouest/glewlwyd/workflows/CodeQL/badge.svg)
[![CII Best Practices](https://bestpractices.coreinfrastructure.org/projects/3475/badge)](https://bestpractices.coreinfrastructure.org/projects/3475)
[![online demo](https://img.shields.io/badge/-Online%20Demo-blue)](/glewlwyd-demo)
[![User documentation](https://img.shields.io/badge/-User%20documentation-green)](/user-profile)

**[Online demo](/glewlwyd-demo)** functionalities available:

**[Register a new user](https://glewlwyd.babelouest.io/index.html?register=regis)**, authentication factors allowed:
- Password [(documentation)](/user-profile#update-password)
- External provider (Google, GitHub or GitLab) [(documentation)](/user-profile#external-oauth2oidc-login)
- HOTP or TOTP [(documentation)](/user-profile#otp)
- Webauthn device (FIDO2, Android or Apple phone or tablet) [(documentation)](/user-profile#webauthn)

**[Test multiple factor authentication](/glewlwyd-demo)**

**Beware!**

Due to the curse of Culhwch and Olwen, the online demo has a limited memory and tends to permanently remove all registered users and clients once every week, on the day of the sun, at Dusk.
Don't expect your registration data to be too persistent!

If the account is not removed yet, you can manually remove it in the [profile page](https://glewlwyd.babelouest.io/), by clicking on the `Delete account` red button.

![demo](/img/login-nopassword.png)

<hr/>

**Contributions are welcome!**

[![Install documentation](https://img.shields.io/badge/-Install%20documentation-green)](https://github.com/babelouest/glewlwyd/blob/master/docs/INSTALL.md)
[![Open an issue](https://img.shields.io/badge/-Open%20an%20issue-green)](https://github.com/babelouest/glewlwyd/issues)

Visit the IRC channel #glewlwyd on the [Libera.​Chat](https://libera.chat/) network.

<hr/>

## Articles tagged with `glewlwyd`:

<div>
  <ul class="list-group">
  
  {% for post in site.posts %}
    {% if post.tags contains 'glewlwyd' %}
      <li class="list-group-item">
        {%- assign date_format = site.minima.date_format | default: "%b %-d, %Y" -%}
        <span class="">{{ post.date | date: date_format }}</span>
        <h5>
          <a class="" href="{{ post.url | relative_url }}">
            {{ post.title | escape }}
          </a>
          {%- for tag in post.tags -%}
            <small class="text-muted">
              <a href="/tags/{{ tag }}" class="tag-link">[{{ tag }}]</a>
            </small>
          {%- endfor -%}
        </h5>
        {%- if site.show_excerpts -%}
          {{ post.excerpt }}
        {%- endif -%}
      </li>
    {% endif %}
  {% endfor %}
  
  </ul>

</div>
