---
layout: default
title: Ulfius
permalink: /ulfius/
---

# Ulfius

[![GitHub release (latest by date)](https://img.shields.io/github/v/release/babelouest/ulfius?style=plastic)](https://github.com/babelouest/ulfius/releases/latest)
[![GitHub last commit](https://img.shields.io/github/last-commit/babelouest/ulfius)](https://github.com/babelouest/ulfius/)
![GitHub](https://img.shields.io/github/license/babelouest/ulfius?style=plastic)
![CodeQL](https://github.com/babelouest/ulfius/workflows/CodeQL/badge.svg)
[![CII Best Practices](https://bestpractices.coreinfrastructure.org/projects/3195/badge)](https://bestpractices.coreinfrastructure.org/projects/3195)

Ulfius is a REST API framework written in C. You can use it to easily create webservices.

This project was created at first to facilitate the development of my [house automation system](https://github.com/babelouest/angharad). My first attempt used [GNU libmicrohttpd](https://www.gnu.org/software/libmicrohttpd/) only. Then when I started another webservice project, I realized that using libmicrohttpd's API was powerful, but took a lot of time to gather request parameters and build the response.

So I started to write a web framework on top of libmicrohttpd, this framework would make it easy to write web applications by giving tools for the developper to focus on its application, and not on the HTTP protocol beneath.

With Ulfius, the developper will write callback functions, these functions will be called when an HTTP request with a given form (method+url) is met.

## Functionalities

Ulfius provides the following functionalities you can use in your webservices:
- Multiple callback functions for the same url
- Data streaming in response
- Websocket service
- Send HTTP requests
- Create a client websocket

## Projects using Ulfius

- [Glewlwyd](https://github.com/babelouest/glewlwyd), a lightweight SSO server that provides OAuth2 and OpenID Connect authentication protocols
- [Le Biniou](https://biniou.net/), user-friendly yet powerful music visualization / VJing tool
- [Angharad](https://github.com/babelouest/angharad), House automation system for ZWave and other types of devices
- [Hutch](https://github.com/babelouest/hutch), a safe locker for passwords and other secrets, using JavaScript client side encryption only
- [Taliesin](https://github.com/babelouest/taliesin), a lightweight audio streaming server
- [Taulas Raspberry Pi Serial interface](https://github.com/babelouest/taulas/tree/master/taulas_raspberrypi_serial), an interface for Arduino devices that implement [Taulas](https://github.com/babelouest/taulas/) protocol, a house automation protocol for Angharad

[![Install documentation](https://img.shields.io/badge/-Install%20documentation-green)](https://github.com/babelouest/ulfius/blob/master/INSTALL.md)
[![Human friendly API documentation](https://img.shields.io/badge/-Human%20friendly%20API%20documentation-green)](https://github.com/babelouest/ulfius/blob/master/API.md)
[![Doxygen API documentation](https://img.shields.io/badge/-Doxygen%20API%20documentation-green)](https://babelouest.github.io/ulfius/)
[![Open an issue](https://img.shields.io/badge/-Open%20an%20issue-green)](https://github.com/babelouest/ulfius/issues)

Visit the IRC channel #ulfius on the [Libera.​Chat](https://libera.chat/) network.

[To the tutorials list](/ulfius-tutorials)

## Articles tagged with `ulfius`:

<hr/>
<div>
  <ul class="list-group">
  
  {% for post in site.posts %}
    {% if post.tags contains 'ulfius' %}
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
