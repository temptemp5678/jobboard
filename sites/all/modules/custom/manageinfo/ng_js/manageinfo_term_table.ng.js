/**
 * AngularJS
 */
var basePathUrl = Drupal.settings.basePath;
var currentLanguagePrefix = Drupal.settings.currentLanguagePrefix;

var basePathUrlPrefix = basePathUrl;
if (currentLanguagePrefix) {
  basePathUrlPrefix = basePathUrl + currentLanguagePrefix + '/';
};

var app = angular.module('termTable', ['ngAnimate']);

// get JSON data
app.factory("mainData", ['$http', function($http) {
  var obj = {};

  // multiple language for products json file
  obj.fetchTermJson = function() {
    var jsonfile = basePathUrlPrefix + 'manage/json/term/table';
    return $http.get(jsonfile);
  }

  return obj;
}]);

/** - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  */

app.controller('TermTableCtrl', ['$scope', '$http', 'mainData',
  function($scope, $http, mainData) {
    $scope.tableData = [];
    mainData.fetchTermJson().success(function(data) {
      $scope.tableData = data;
      console.log($scope.tableData);
    });
  }
]);

