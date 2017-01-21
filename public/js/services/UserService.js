angular.module('UserService', []).factory('User', ['$http', function($http) {
  return {
    get: function() {
      return $http.get('/api/users');
    }
  }
}]);
