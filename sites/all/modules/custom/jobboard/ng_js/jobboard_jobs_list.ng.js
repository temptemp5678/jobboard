/**
 * AngularJS
 */
var basePathUrl = Drupal.settings.basePath;

var app = angular.module('orderSummary', ['ngAnimate']);
app.controller('orderSummaryController', ['$scope', '$http', '$sce', function($scope, $http, $sce) {

  $http.get(basePathUrl + 'jsoninfo/jobboard/jobs/list')
    .success(function (data) {
       $scope.orderTableRow = data.nodes.order;
    })
    .error(function (data, status, headers, config) {
       //  Do some error handling here
    });

  /**
   * hover show row details
   */
  $scope.hoverIn = function(){
    this.hoverInStatus = true;
  };
  $scope.hoverOut = function(){
    this.hoverInStatus = false;
  };

  /**
   * shows HTML code within the tag
   */
  $scope.renderHtml = function(html_code) {
    return $sce.trustAsHtml(html_code);
  };

  /**
   * temp array to store selected order list
   */
  $scope.selectedOrders = [];
  $scope.selectedOrdersIdArray = [];

  $scope.addOrderList = function(inputValue) {
    $scope.selectedOrders.push(inputValue);
    }

  $scope.removeOrderList = function (index) {
    if (index > -1) {
      $scope.selectedOrders.splice(index, 1);
    }
  }

  /**
   * submit
   */
  $scope.toQuoteBtn = function (index) {

    var selectedOrdersId = [];
    angular.forEach($scope.selectedOrders, function(value, key) {
      // only push unique item
      if(this.indexOf(value.nodeNid) == -1) {
        this.push(value.nodeNid);
      }
    }, selectedOrdersId);

    var selectedDashOrders = selectedOrdersId.join("-");
    var json = angular.toJson($scope.fields);
    var redirectPageUrl = basePathUrl + 'quote/import/orders/' + selectedDashOrders;

    window.location.replace(redirectPageUrl);
  }

}]);

