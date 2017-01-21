var User = require('./models/User');

module.exports = function(app) {
  app.get('/api/users', function(req, res) {
    User.find(function(err, users) {
      if(err)
        res.send(err);

      res.json(users);
    });
  });

  // All other routes, serve main page
  app.get('*', function(req, res) {
    res.sendfile('./public/views/index.html');
  });
}
