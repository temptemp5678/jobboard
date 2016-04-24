angular.module('flexxiaGoogleMapModule', []).directive('flexxiagooglemap', function() {
  link = function(scope, element, attrs) {
    // Drupal.behaviors.zencharts = {
    // attach: function(context, settings) {
    /**
     * Google Map show meeting info
     */
    var infoBubble = new InfoBubble({
      maxHeight: 385,
      maxWidth: 510,
      disableAutoPan: false,
      shadowStyle: 1,
      padding: '0px',
      borderRadius: 5,
      minWidth: 20,
      minHeight: 510,
      arrowSize: 10,
      hideCloseButton: false,
      arrowPosition: 7,
      backgroundClassName: 'phoney',
      pixelOffset: new google.maps.Size(130, 120),
      arrowStyle: 2,
      content: ''
    });
    var siteBasePath = Drupal.settings.basePath;
    var latitudeInCluster = ' ';
    var longitudeInCluster = ' ';
    var similarLatitude = [];
    var similarLongitude = [];
    var latLngCount = 0;
    var storeLatCount = [];
    var speakerNameDataTables = [];
    var meetingDateDataTables = [];
    var speakerEvaluationLinkDataTables = [];
    var center = new google.maps.LatLng(53.508742, -90.120850);
    var options = {
      'zoom': 3,
      'center': center,
      'mapTypeId': google.maps.MapTypeId.ROADMAP,
      'streetViewControl': false,
      'panControl': false,
      'mapTypeControl': false,
    };
    /**
     * map cluster style
     */
    var urlPath = siteBasePath + 'sites/all/libraries/google_map/images/mapmaker/';
    var clusterStyles = [{
      textColor: '#000000',
      url: urlPath + 'm3.png',
      height: 45,
      width: 30,
      // anchorIcon: [35, 10],                 // commented  intensionally to demo
      anchorText: [-7, 0]
    }, {
      textColor: '#000000',
      url: urlPath + 'map_marker_blue.png',
      height: 55,
      width: 38,
      // anchorIcon: [35, 10],                  // commented  intensionally to demo
      anchorText: [-10, -2]
    }, {
      textColor: '#000000',
      url: urlPath + 'map_marker_red.png',
      height: 42,
      width: 42,
      // anchorIcon: [60, 10],                  // commented intensionally to demo
      //anchorText: [-7, 0]                     // commented intensionally to demo
    }];
    var markers = [];
    var mapClusterOptions = {
      gridSize: 50,
      styles: clusterStyles,
      maxZoom: 15,
    };
    var map = new google.maps.Map(document.getElementById("map_div"), options);
    /**
     * Full screen mode library call
     */
    // map.controls[google.maps.ControlPosition.TOP_RIGHT].push(new FullScreenControl(map));
    var markerImageSingleMeeting = siteBasePath + 'sites/all/libraries/google_map/images/' + 'marker_icon.png'; //single meeting icon image path
    var markerImageMultipleMeetings = siteBasePath + 'sites/all/libraries/google_map/images/' + 'marker_icon.png'; //multiple meetings icon image path
    var jsonPath = siteBasePath + 'sites/default/files/json/source/' + 'map_meeting_list.json'; // fetching data from json file
    var jsonFile = null;
    jQuery.ajax({
      'async': false,
      'global': false,
      'url': jsonPath,
      'dataType': "json",
      'success': function(data) {
        jsonFile = data;
      }
    });
    if (jsonFile == null) {
      return;
    }
    /**
     * markers mouse-over
     */
    jQuery.each(jsonFile, function(key, value) {
      var latitude = Number(value.lat).toFixed(3);
      var longitude = Number(value.lng).toFixed(3);
      var unitInfoPopupHtml = "";
      if (jQuery.inArray(value.lat, similarLatitude) == -1) {
        similarLatitude.push(value.lat);
      }
      if (jQuery.inArray(value.lng, similarLongitude) == -1) {
        similarLongitude.push(value.lng);
      }
    });
    /**
     * getting distinct latitudes and longitudes
     */
    for (var i = 0; i <= similarLatitude.length; i++) {
      latLngCount = 0;
      var speakerNameDataTables = [];
      var speakerNameUrlTables = [];
      var meetingDateDataTables = [];
      var speakerEvaluationLinkDataTables = [];
      var latitude = Number(similarLatitude[i]).toFixed(3);
      var longitude = Number(similarLongitude[i]).toFixed(3);
      /**
       * separating the latLongCount with more than one meeting
       */
      jQuery.each(jsonFile, function(key, value) {
        if ((latitude == Number(value.lat).toFixed(3)) && (longitude == Number(value.lng).toFixed(3))) {
          latLngCount = latLngCount + 1;
          if (latLngCount == 1) {
            /**
             * In case of Single meeting
             */
            speakerNameDataTables.push(value.speakerName);
            meetingDateDataTables.push(value.meetingDate);
            speakerNameUrlTables.push(value.speakerNameUrl);
            speakerEvaluationLinkDataTables.push(value.speakerEvaluationLink);
            var latLng = new google.maps.LatLng(latitude, longitude);
            var marker = new google.maps.Marker({
              position: latLng,
              map: map,
              icon: markerImageSingleMeeting // Commented intentionally
            });
            var unitName = value.unitName;
            var programName = value.programName;
            var speakerName = value.speakerName;
            var repName = value.repName;
            var meetingDate = value.meetingDate;
            var meetingLocation = value.meetingLocation;
            var evatuationCount = value.evaluationCount;
            var speakerEvaluationLink = value.speakerEvaluationLink;
            var unitInfoPopupHtml = " ";
            /**
             * HTML to display single meeting info
             * value.unitName
             */
            unitInfoPopupHtml += '<div id="wrapper" style="overflow:hidden; white-space:nowrap;">';

              unitInfoPopupHtml += '<div class="col-md-12" style="width:100%; min-width:430px; max-width:430px; background:#0082ba; min-height:45px; margin-bottom:20px;">';
                unitInfoPopupHtml += '<div>';
                unitInfoPopupHtml += '<div class="col-md-12" style="color:#ffffff; margin-left:3%; font-size:16px; padding-top:12px; "><span>' + value.programName + '</span></div>';
                unitInfoPopupHtml += '</div>';
              unitInfoPopupHtml += '</div>';

              unitInfoPopupHtml += '<div class="row" style="margin-top:15px; padding-left:14px;">';
                unitInfoPopupHtml += '<div class="col-md-12 ">';
                  unitInfoPopupHtml += '<div style="padding: 0px" class="col-md-4 margin-top-6 font-size-14 color-0082ba">';
                    unitInfoPopupHtml += '<span class="fa fa-bookmark padding-6"></span>';
                    unitInfoPopupHtml += '<span style="color:#000000; padding-top:0.5px; font-size:14px; ">' + value.unitName + '</span>';
                  unitInfoPopupHtml += '</div>';
                  unitInfoPopupHtml += '<div style="padding: 0px" class="col-md-4 margin-top-6 font-size-14 color-0082ba padding-0">';
                    unitInfoPopupHtml += '<span class="fa fa-calendar padding-6"></span>';
                    unitInfoPopupHtml += '<span style="color:#000000; padding-top:0.5px; font-size:14px; ">' + value.meetingDate + '</span>';
                  unitInfoPopupHtml += '</div>';
                  unitInfoPopupHtml += '<div style="padding: 0px" class="col-md-4 margin-top-6 font-size-14 color-0082ba padding-0">';
                    unitInfoPopupHtml += '<span class="fa fa-map-marker padding-6"></span>';
                    unitInfoPopupHtml += '<span style="color:#000000; padding-top:0.5px; font-size:14px; ">' + value.meetingLocation + '</span>';
                  unitInfoPopupHtml += '</div>';
                unitInfoPopupHtml += '</div><br /><br />';
                unitInfoPopupHtml += '<hr>';

                unitInfoPopupHtml += '<div class="row">';
                  unitInfoPopupHtml += '<div class="col-md-12">';
                    unitInfoPopupHtml += '<span style="color:rgba(0,0,0,0.54); padding-top:3px; font-size:14px;">Speaker: </span>';
                      var guestpage = jQuery('.guestpage-program-heading');
                      if (guestpage.length > 0) {
                        unitInfoPopupHtml += '<span style="padding-top:0.5px; font-size:17px; ">' + value.speakerName + '</span>';
                      }
                      else {
                        unitInfoPopupHtml += '<span class="speakerNameSpan" id="speakerNameId" href="' + value.speakerNameUrl + '/#speaker-evaluation-summary-wrapper" style="font-size:14px; ">' + value.speakerName + '</span>';
                      }
                    unitInfoPopupHtml += '</div>';
                    unitInfoPopupHtml += '<div class="col-md-12 margin-top-12">';
                    unitInfoPopupHtml += '<span style="color:rgba(0,0,0,0.54); padding-top:3px; font-size:14px;">Rep: </span>';
                      unitInfoPopupHtml += '<span style="color:#000000; padding-top:0.5px; font-size:14px; ">' + value.repName + '</span>';
                    unitInfoPopupHtml += '</div>';
                    unitInfoPopupHtml += '<div class="col-md-12 margin-top-12">';
                    unitInfoPopupHtml += '<span style="color:rgba(0,0,0,0.54); padding-top:3px; font-size:14px;">Venue: </span>';
                      unitInfoPopupHtml += '<span style="color:#000000; padding-top:0.5px; font-size:14px; ">' + value.venuName + '</span>';
                    unitInfoPopupHtml += '</div>';
                    unitInfoPopupHtml += '<div class="col-md-12 margin-top-12">';
                    unitInfoPopupHtml += '<span style="color:rgba(0,0,0,0.54); padding-top:3px; font-size:14px;">Speaker Evaluation: </span>';
                      unitInfoPopupHtml += '<span style="color:#000000; padding-top:0.5px; font-size:14px; ">' + value.eventLink + '</span>';
                    unitInfoPopupHtml += '</div>';
                  unitInfoPopupHtml += '</div>';
                unitInfoPopupHtml += '<hr>';

                unitInfoPopupHtml += '<div class="col-md-12 text-center">';
                  unitInfoPopupHtml += '<div class="col-md-6 margin-top-6">';
                    unitInfoPopupHtml += '<span style="color:rgba(0,0,0,0.54); padding-top:3px; font-size:18px;">ATTENDEES</span>';
                  unitInfoPopupHtml += '</div>';
                  unitInfoPopupHtml += '<div class="col-md-6 margin-top-6">';
                    unitInfoPopupHtml += '<span style="color:rgba(0,0,0,0.54); padding-top:3px; font-size:18px;">RESPONSES</span>';
                  unitInfoPopupHtml += '</div>';
                  unitInfoPopupHtml += '<div class="col-md-6 margin-top-6">';
                    unitInfoPopupHtml += '<span style="color:#0082ba; padding-top:0.5px; font-size:24px; font-weight:bold;">' + value.attendiesCount + '</span>';
                  unitInfoPopupHtml += '</div>';
                  unitInfoPopupHtml += '<div class="col-md-6 margin-top-6">';
                    unitInfoPopupHtml += '<span style="color:#0082ba; padding-top:0.5px; font-size:24px; font-weight:bold;">' + value.evaluationCount + '</span>';
                  unitInfoPopupHtml += '</div>';
                unitInfoPopupHtml += '</div><br />';
              unitInfoPopupHtml += '</div>';

            unitInfoPopupHtml += '</div>';
            bindInfoWindow(marker, map, infoBubble, unitInfoPopupHtml);
          } else if (latLngCount > 1) {
            /**
             * In case multiple meetings on one lat lng
             */
            speakerNameDataTables.push(value.speakerName);
            meetingDateDataTables.push(value.meetingDate);
            speakerNameUrlTables.push(value.speakerNameUrl);
            speakerEvaluationLinkDataTables.push(value.speakerEvaluationLink);
            var latLng = new google.maps.LatLng(latitude, longitude);
            var marker = new google.maps.Marker({
              position: latLng,
              map: map,
              icon: markerImageMultipleMeetings // commented intensionally
            });
            /**
             * HTML to display multiple meeting table
             */
            unitInfoPopupHtml = '<table id="mapTables" class="table table-striped" cellspacing="0" width="100%">';
            unitInfoPopupHtml += '<thead>';
            unitInfoPopupHtml += '<tr>';
            unitInfoPopupHtml += '<th>Sr</th>';
            unitInfoPopupHtml += '<th>Speaker</th>';
            unitInfoPopupHtml += '<th>Date</th>';
            unitInfoPopupHtml += '<th>Evaluation</th>';
            unitInfoPopupHtml += '</tr>';
            unitInfoPopupHtml += '</thead>';
            for (var i = 0; i <= speakerNameDataTables.length - 1; i++) {
              unitInfoPopupHtml += '<tr>';
              var x = Number(i);
              unitInfoPopupHtml += '<th>' + Number(x + 1) + '</th>';
              unitInfoPopupHtml += '<th class="speakerNameSpan" href="' + value.speakerNameUrl + '/#speaker-evaluation-summary-wrapper" style="cursor:pointer; color:#4b91ce;   font-weight:bold;">' + speakerNameDataTables[i] + '</th>';
              unitInfoPopupHtml += '<th>' + meetingDateDataTables[i] + '</th>';
              unitInfoPopupHtml += '<th>' + speakerEvaluationLinkDataTables[i] + '</th>';
              unitInfoPopupHtml += ' </tr>';
            }
            unitInfoPopupHtml += '</table>';
            bindInfoWindow(marker, map, infoBubble, unitInfoPopupHtml);
          }
          markers.push(marker); // Pushing markers into marker variable
        }
      });
    }
    /**
     * operations on marker cluster
     */
    var markerCluster = new MarkerClusterer(map, markers, mapClusterOptions);
    /**
     * Marker cluster event function starts
     */
    google.maps.event.addListener(markerCluster, 'mouseover', function(c) {
      var content = '';
      // Convert lat/long from cluster object to a usable MVCObject
      var info = new google.maps.MVCObject;
      info.set('position', c.center_);
      // Get markers
      var fetchMarkers = c.getMarkers();
      var markerInCluster = [];
      var uniqueLatitude = [];
      var uniqueLongitude = [];
      var uniqueUnitName = [];
      var uniqueProgramName = [];
      var unitNameCountArray = [];
      var programNameCountArray = [];
      var checkPageUrlArray = [];
      var titles = " ";
      var unitNameInCluster = ' ';
      var programNameInCluster = ' ';
      var unitNameCount = 0;
      var unitNeuroscienceCount = 0;
      var unitDiabetesCount = 0;
      var unitLillyAllianceCount = 0;
      var flagUnit = 0;
      var flagUnit1 = 0;
      //console.log(fetchMarkers);
      for (var i = 0; i < fetchMarkers.length; i++) {
        markerInCluster.push(fetchMarkers[i].getPosition());
      }
      jQuery.each(markerInCluster, function(key, value) {
        flagUnit = flagUnit + 1;
        flagUnit1 = flagUnit1 + 1;
        /**
         * Here value.k and value.D is used because of predefined array structure latitude and longitude
         * load library "Google Map API" specify v=3.19
         */
        // Converting Latitude Longitude Object into Array
        var latLngArray = jQuery.map(value, function(value, index) {
          return [value];
        });
        latitudeInCluster = Number(latLngArray[0]).toFixed(3);
        longitudeInCluster = Number(latLngArray[1]).toFixed(3);
        //console.log(latitudeInCluster);
        if (isNaN(latitudeInCluster)) {
          console.log('Google library is Updated. Please contact the Administrator');
        }
        /**
         * Fetching distinct lat lng
         */
        jQuery.each(jsonFile, function(key, value) {
          if (jQuery.inArray(value.lat, uniqueLatitude) == -1) {
            uniqueLatitude.push(value.lat);
          }
          if (jQuery.inArray(value.lng, uniqueLongitude) == -1) {
            uniqueLongitude.push(value.lng);
          }
          if (jQuery.inArray(value.unitName, uniqueUnitName) == -1) {
            uniqueUnitName.push(value.unitName);
            checkPageUrlArray.push(value.pageUrl);
          }
          if (jQuery.inArray(value.programName, uniqueProgramName) == -1) {
            uniqueProgramName.push(value.programName);
          }
        });
        if (uniqueUnitName.length > 0 && flagUnit == 1) {
          for (var i = 0; i <= uniqueUnitName.length - 1; i++) {
            unitNameCountArray[i] = 0;
          }
        }
        if (uniqueProgramName.length > 0 && flagUnit1 == 1) {
          for (var i = 0; i <= uniqueProgramName.length - 1; i++) {
            programNameCountArray[i] = 0;
          }
        }
        for (var x = 0; x <= uniqueLatitude.length; x++) {
          var latitude = Number(uniqueLatitude[x]).toFixed(3);
          var longitude = Number(uniqueLongitude[x]).toFixed(3);
          /**
           * Info window for clusters
           */
          if (latitude == latitudeInCluster && longitude == longitudeInCluster) {
            jQuery.each(jsonFile, function(key1, value1) {
              if (latitudeInCluster == Number(value1.lat).toFixed(3) && longitudeInCluster == Number(value1.lng).toFixed(3)) {
                unitNameInCluster = value1.unitName;
                programNameInCluster = value1.programName;
              }
            });
            for (i = 0; i <= uniqueUnitName.length - 1; i++) {
              if (unitNameInCluster == uniqueUnitName[i]) {
                unitNameCountArray[i] = unitNameCountArray[i] + 1;
              }
            }
            for (i = 0; i <= uniqueProgramName.length - 1; i++) {
              if (programNameInCluster == uniqueProgramName[i]) {
                programNameCountArray[i] = programNameCountArray[i] + 1;
              }
            }
          }
        }
      });
      for (var i = 0; i <= uniqueUnitName.length - 1; i++) {
        if (checkPageUrlArray[i] == 'program-project') {
          if (unitNameCountArray[i] > 0) {
            titles += '<p>' + uniqueUnitName[i] + ' : ' + unitNameCountArray[i] + '</p>';
          }
        } else {
          for (var i = 0; i <= uniqueProgramName.length - 1; i++) {
            if (programNameCountArray[i] > 0) {
              titles += '<p>' + uniqueProgramName[i] + ' : ' + programNameCountArray[i] + '</p>';
            }
          }
        }
      }
      var infowindow = new google.maps.InfoWindow();
      /**
       * temporary disable info window to debug
       */
      // infowindow.setContent(titles); //set info window content to titles
      // infowindow.open(map, info);
      // google.maps.event.addListener(markerCluster, 'mouseout', function(c) {
      //   infowindow.close();
      // });
    }); // --------end for Marker cluster event: function()
    /**
     * Info window function to display single or multiple meetings
     */
    function bindInfoWindow(marker, map, infoBubble, html) {
      google.maps.event.addListener(marker, 'mouseover', function() {
        infoBubble.setContent(html);
        infoBubble.open(map, marker);
        /**
         * fancy box call to display Speaker Info
         */
        // jQuery(infoBubble.e).find(".speakerNameSpan").live("click", function(event) {
        //  jQuery(".speakerNameSpan").fancybox ({
        //    'padding': 20,
        //    'margin': 30,
        //    'width': 771,
        //    'height': 615,
        //    'autoScale': false,
        //    'overlayShow': true,
        //    'autoDimensions': false,
        //    'transitionIn': 'fade',
        //    'transitionOut': 'fade',
        //    'type': 'iframe',
        //    'scrolling': 'auto'
        //  }); //---end for fancy box function()
        // }); //-----end for click event info bubble function()
      }); //-------end for bindInfoWindow function()
      google.maps.event.addListener(marker, 'mouseout', function() {});
    } // ----------end for bindInfoWindow: function()
    // } // ------------end for attach: function()
    // };
  }
  return {
    restrict: 'A',
    // template: '<div id="map_div" style="height: 600px;"></div>',
    replace: true,
    link: link
  };
});