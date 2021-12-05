---
layout: default
title: Ulfius
permalink: /ulfius-tutorials/
---

[Back to Ulfius page](/ulfius)

# Ulfius tutorials

If you have other ideas for tutorials or extended documentation, let me know in the [discussion section](https://github.com/babelouest/ulfius/discussions) on GitHub, or send me an [e-mail](mailto:info@babelouest.io)
<hr/>
<div>
  <ul class="list-group">
  
  {% for post in site.posts reversed %}
    {% if post.tags contains 'ulfius' and post.tags contains 'tutorial' %}
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
