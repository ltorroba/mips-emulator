var express = require('express');
var app = express();
var bodyParser = require('body-parser');
var methodOverride = require('method-override');

var db = require('./config/db');

var port = process.env.PORT || 8080;

//mongoose.connect(db.url);

// Parse application/json
app.use(bodyParser.json());

// Parse application/vnd.api+json as json
app.use(bodyParser.json({ type: 'application/vnd.api+json' }));

// Parse application/x-www-form-urlencoded
app.use(bodyParser.urlencoded({ extended: true }));

// Override with X-HTTP-Method-Override
app.use(methodOverride('X-HTTP-Method-Override'));

// Set static files location
app.use(express.static(__dirname + '/public'));

require('./server/routes')(app);

app.listen(port);

console.log('Listening on port ' + port);

exports = module.exports = app;
