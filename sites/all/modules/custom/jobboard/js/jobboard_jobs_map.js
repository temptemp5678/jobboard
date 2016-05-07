jQuery(document).ready(function(){

  var basePathUrl = Drupal.settings.basePath;
  var map;
  var ajaxRequest;
  var plotlist;
  var plotlayers=[];

  // initialize the map
  var map = L.map('mapid').setView([42.26, -82.95], 12);             // Windsor
  // var map = L.map('mapid').setView([42.35, -71.08], 11);          // Boston
  // var map = L.map('mapid').setView([39.911640, 116.401938], 12);  // Beijing

  // load a tile layer
  L.tileLayer('http://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png',
   {
     attribution: 'Map data © <a href="http://openstreetmap.org">OpenStreetMap</a> contributors',
     maxZoom: 17,
     minZoom: 10
   }).addTo(map);

  /** - - - - - - - Numbered Markers - - - - - - - - - - - - - - - - - - - - - - - -  */
  /**
   * Numbered Markers in Leaflet (JS Mapping)
   * https://gist.github.com/comp615/2288108
   */
  L.NumberedDivIcon = L.Icon.extend({
    options: {
      // iconUrl: '<%= image_path("leaflet/marker_hole.png") %>',
      iconUrl: basePathUrl + 'sites/all/modules/custom/jobboard/image/marker_hole_44a3d3.png',
      number: '',
      shadowUrl: null,
      iconSize: new L.Point(25, 41),
      iconAnchor: new L.Point(13, 41),
      popupAnchor: new L.Point(0, -33),
      /*
      iconAnchor: (Point)
      popupAnchor: (Point)
      */
      className: 'leaflet-div-icon'
    },

    createIcon: function () {
      var div = document.createElement('div');
      var img = this._createImg(this.options['iconUrl']);
      var numdiv = document.createElement('div');
      numdiv.setAttribute ( "class", "number" );
      numdiv.innerHTML = this.options['number'] || '';
      div.appendChild ( img );
      div.appendChild ( numdiv );
      this._setIconStyles(div, 'icon');
      return div;
    },

    //you could change this to add a shadow like in the normal marker if you really wanted
    createShadow: function () {
      return null;
    }
  });

  /** - - - - - - - Markers Coordinates- - - - - - - - - - - - - - - - - - - - - - - -  */
  /**
   * fake one
   */
  var windsorMarker = [
    {
      coordinates: [42.33, -82.95],
      popupText: "I am a polygon.",
    },
    {
      coordinates: [42.31, -82.99],
      popupText: "I am a polygon2.",
    }
  ];

  /** - - - - - - -JSON - - - - - - - - - - - - - - - - - - - - - - - -  */
  jsonUrl = basePathUrl + 'jsoninfo/jobboard/jobs/map';
  var promise = jQuery.getJSON(jsonUrl);
  promise.then(function(data) {
    // console.log(data) // take a look at the data in the console

    var windsorMarker = data;

    // loop each point to add marker to map
    for (var i = 0; i < windsorMarker.length; i++) {
      new L.Marker(windsorMarker[i].coordinates, {
        icon: new L.NumberedDivIcon({number: (i + 1) })
      }).addTo(map).bindPopup(windsorMarker[i].popupText);
    }

    /** - - - - - - - draw a polyline in Leaflet - - - - - - - - - - - - - - - - - - -  */
    // var pointList = [
    //   new L.LatLng(42.33, -82.95),
    //   new L.LatLng(42.33, -82.85),
    // ];

    // var firstpolyline = new L.Polyline(pointList, {
    //   color: 'red',
    //   weight: 3,
    //   opacity: 0.3,
    //   smoothFactor: 1
    // });
    // firstpolyline.addTo(map);
  });





});
