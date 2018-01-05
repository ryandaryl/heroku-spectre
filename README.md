# Spectre proof of concept on Heroku

The C implementation of a simple spectre attack by [Eugnis](https://github.com/Eugnis/spectre-attack). I've added a http server and directed the log output to the browser.

See it working [here](http://spectre-rdm.herokuapp.com).

The C script is compiled using the Heroku C buildpack. Once you've created an app with the C [buildpack](https://github.com/heroku/heroku-buildpack-c), you can...

## Deploy to Heroku

[![Deploy](https://www.herokucdn.com/deploy/button.png)](https://heroku.com/deploy)

(The requirement for the C buildpack, which is not automatically detected, and my inability to get app.json to work, means you will have to deploy use Heroku CLI. Don't use the button above, which is just for visual interest right now.)