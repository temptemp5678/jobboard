jQuery(document).ready(function() {

  var basePathUrl = Drupal.settings.basePath;
  // var jsonFilePath = basePathUrl + 'api/json/node/all-product-data';

  // /**
  //  * fetch JSON, return json file
  //  */
  // var fetchJson = function (myUrl) {
  //   var json = null;
  //   jQuery.ajax({
  //     'async': false,
  //     'global': false,
  //     'url': myUrl,
  //     'dataType': "json",
  //     'success': function (data) {
  //       json = data.nodes.products;
  //     }
  //   });
  //   return json;
  // };

  // var allProductTable = fetchJson(jsonFilePath);
  var allProductTable = [
    {
      nodeTitle: 'Entry Level',
      topPriority: 12,
    },
    {
      nodeTitle: 'Executive',
      topPriority: 22,
    },
    {
      nodeTitle: 'Experienced',
      topPriority: 18,
    },
    {
      nodeTitle: 'Management',
      topPriority: 6,
    },
    {
      nodeTitle: 'Student',
      topPriority: 2,
    },
  ];


  var chartjsDataLabel = [];
  var chartjsDataValue = [];

  for (var i = 0; i < 5; i++) {
    chartjsDataLabel.push(allProductTable[i].nodeTitle);
    chartjsDataValue.push(allProductTable[i].topPriority);
  }

  /*
   * Bar chart
   */
  Chart.defaults.global.tooltipTitleFontFamily = '"Open Sans", "Helvetica Neue", Helvetica, Arial, sans-serif';
  Chart.defaults.global.scaleFontSize = 10;

  var data = {
    labels: chartjsDataLabel,
    datasets: [
      {
        label: "My First dataset",
        data: chartjsDataValue,
      }
    ]
  };

  var options = {
    animation: true,
    barShowStroke : false,
    barValueSpacing : 80,
    responsive: true
  };

  var IDBarChart = jQuery('#reportpage-summary-quote-bar-chart');
  var contextBarChart = IDBarChart.get(0).getContext('2d');

  var myBarChart = new Chart(contextBarChart).Bar(data,options);
  //Bar Colors
  myBarChart.datasets[0].bars[0].fillColor = "#b6da52"; //bar 1
  myBarChart.datasets[0].bars[1].fillColor = "#3da5e7"; //bar 2
  myBarChart.datasets[0].bars[2].fillColor = "#e72682"; //bar 3
  myBarChart.datasets[0].bars[3].fillColor = "#edc351"; //bar 3
  myBarChart.datasets[0].bars[4].fillColor = "#aac1cc"; //bar 3
  // myBarChart.datasets[0].bars[5].fillColor = "#b6da52"; //bar 3
  // myBarChart.datasets[0].bars[6].fillColor = "#3da5e7"; //bar 3
  // myBarChart.datasets[0].bars[7].fillColor = "#edc351"; //bar 3
  myBarChart.update();

  /*
   * Line chart
   */
  Chart.defaults.global.tooltipTitleFontFamily = '"Open Sans", "Helvetica Neue", Helvetica, Arial, sans-serif';
  Chart.defaults.global.scaleFontSize = 10;

  var data = {
    labels: ["2015-Q1", "2015-Q2", "2015-Q3", "2015-Q4", "2016-Q1"],
      datasets: [
      {
        label: "Harmonic",
        fillColor: "rgba(0,0,0,0)",
        strokeColor: "#b6da52",
        pointColor: "#b6da52",
        pointStrokeColor: "#fff",
        pointHighlightFill: "#fff",
        pointHighlightStroke: "#b6da52",
        data: [60, 59, 80, 81, 56,]
      },
      {
        label: "NVION",
        fillColor: "rgba(0,0,0,0)",
        strokeColor: "#3da5e7",
        pointColor: "#3da5e7",
        pointStrokeColor: "#fff",
        pointHighlightFill: "#fff",
        pointHighlightStroke: "#3da5e7",
        data: [28, 45, 78, 49, 84,]
      },
      {
        label: "Harris",
        fillColor: "rgba(0,0,0,0)",
        strokeColor: "#e72682",
        pointColor: "#e72682",
        pointStrokeColor: "#fff",
        pointHighlightFill: "#fff",
        pointHighlightStroke: "#e72682",
        data: [65, 59, 50, 61, 56,]
      },
      {
        label: "LiveU",
        fillColor: "rgba(0,0,0,0)",
        strokeColor: "#aac1cc",
        pointColor: "#aac1cc",
        pointStrokeColor: "#fff",
        pointHighlightFill: "#fff",
        pointHighlightStroke: "#aac1cc",
        data: [38, 45, 60, 99, 46,]
      }
    ]
  };
  var options = {
    animation: true,
    barShowStroke : false,
    barValueSpacing : 30,
    bezierCurveTension : 0.1,
    responsive: true,
    multiTooltipTemplate: "<%= datasetLabel %> - <%= value %>",
  };

  var IDLineChart = jQuery('#reportpage-summary-quote-line-chart');
  var contextLineChart = IDLineChart.get(0).getContext('2d');
  var myLineChart = new Chart(contextLineChart).Line(data,options);

});

