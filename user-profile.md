---
layout: default
title: Glewlwyd user profile
permalink: /user-profile/
---

[Back to Glewlwyd page](/glewlwyd)

# Into Glewlwyd profile

This documentation describes the user profile page on [glewlwyd](https://glewlwyd.babelouest.io/), how to update your user's credentials, and how to log in to glewlwyd and how to completely remove your account.

## Update password

![profile-multiple-password](/screenshots/profile-multiple-password.png)

If you click on the `Password` tab, you'll be able to change your user passwords. You can have multiple passwords if you need to, or none if your Glewlwyd service allows passwordless authentication.

Be careful that every password will allow the same access level to your Glewlwyd service, so all of them must be strong enough to avoid being guessed.

## External OAuth2/OIDC Login

![profile-oauth2](/screenshots/profile-oauth2.png)

![login-oauth2](/screenshots/login-oauth2.png)

This scheme allow users to connect to Glewlwyd via an account on a trusted external OAuth2/OIDC service. External OAuth2/OIDC services may include mainstream services such as Google, Facebook, GitHub, etc.

### Registration

You need a valid login in one of the proposed external services. In the Profile page, click on the `+` button of one of the provider, then proceed to the login workflow in the provider service. You may be asked to accept Glewlwyd service to be authorized to access your personal data. You must accept this authorization to allow Glewlwyd to identify your login. Glewlwyd will never have access to secret data such as passwords in this process, and will only save the minimal data (i.e. an identifier) to be able to identify your connection during further login phases.

## OTP

![profile-otp](/screenshots/profile-otp.png)

OTP is an authentication scheme using the OATH standard, it allows the user to authenticate via one-time passwords. Glewlwyd OTP scheme allows HOTP (increment based one-time passwords) and TOTP (time-based one-time passwords).

You'll need to synchronize your OTP parameters with another device so you can authenticate via this scheme. You have several mobile apps available, as well as physical devices available to achieve this.

### Registration

If you want to register a new OTP account, on the `OTP` tab, select the type: `HOTP` (increment based) or `TOTP` (time-based). Usually, TOTP is preferred over HOTP because it's easier to use, with a similar security level. If you want to disable this scheme for your account, select type `None`.

You need to enter a shared secret. This must be a random number, large enough to make it difficult to guess, encoded in Base32 format. If your administrator have provided you a shared secret, type it, or copy/paste it in the text-box. If you don't have one, you can generate a random secret by clicking on the `Generate` button right to the text-box.

#### HOTP parameter

Moving factor: This option is the starting offset of the HOTP configuration. Usually the default value is 0.

#### TOTP parameter

Step size (seconds): This option is the time window specifying the duration of a one-time password. This duration must be long enough to give time to the user to read and type it, but short enough to give an attacker enough time to reuse an already used password. The typical duration is 30 seconds.

## WebAuthn

![profile-webauthn](/screenshots/profile-webauthn.png)

WebAuthn is an authentication scheme that permits the user to authenticate via a dedicated device without having to retain a password. Currently, the devices available for this scheme are the following:

- Yubikeys
- Android devices (phone or tablet) if the system version is Nougat (7.0) or higher
- Apple devices (iPhone or iPad) if the system is iOS 14 or higher

You'll need a browser compatible with WebAuthn API. Recent versions of Firefox, Chrome, Edge and Opera work fine, check [Can I use](https://caniuse.com/#search=webauthn) for more details.

### Registration

On the `WebAuthn` tab, click on the button `New Component`. There, follow your browser instructions. Eventually, you should see the new registered component in the component list.

You can verify it's working properly by clicking on the `Test` button, there, follow your browser's instructions. You should see a popup message saying `Authentication succeeded`.

The component name can be changed in this page, you can also disable/enable, and delete a registered component.

<a href="#login"></a>
## Login to Glewlwyd

![login-new-user](/screenshots/login-new-user.png)

When you're asked to login to an application that uses Glewlwyd as authentication service, you may need to enter credentials, depending on the access required and the server configuration.

During a login process, you may have to use multiple authentication schemes, and choose between them. When this happens, you have a `Scheme` drop-down button available on the bottom-left of the login window. There, you can choose between the schemes available.

### Grant access

![login-grant](/screenshots/login-grant.png)

When you first login to an application via Glewlwyd, you'll be asked to grant access to the client. The client needs your permission to have access to scopes you're allowed to use. You need to grant at least one scope to the client, otherwise the client won't have access. At any time, you can go back to the login page and change the access granted to this client by clicking on the `Grant` menu available in the `Manage` drop-down button of the login page.

### Multiple sessions

![login-multiple-session](/screenshots/login-multiple-session.png)

If you have multiple logins available on the Glewlwyd server, you can switch from one to another without having to logout every time. The login available are specific to a browser and a session. Use the drop-down button `Change user` on the bottom-left of the login page.

### Authentication complete

![logged-in](/screenshots/logged-in.png)

If the authentication process is complete, you'll have a `Continue` button available. By clicking on this button, you'll be redirected to the client with credentials available to the client.

<a href="#delete"></a>
## Delete account

If available, you can delete your own account on Glewlwyd. You need to be connected to the profile page. In the `Personal data` tab, you have a `Delete account` button on the bottom left of the screen. By clicking on this button, you will delete your account and all the schemes registration attached to your account.
