jQuery(document).ready(function(){

  var basePathUrl = Drupal.settings.basePath;
  var map;
  var ajaxRequest;
  var plotlist;
  var plotlayers=[];

  // function initmap() {
  //   // set up the map, initialize the map object
  //   map = new L.Map('mapid');

  //   // create the tile layer with correct attribution
  //   var osmUrl = 'http://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png';

  //   var osmAttrib = 'Map data © <a href="http://openstreetmap.org">OpenStreetMap</a> contributors';

  //   // Used L.tileLayer() to create a base layer of map tiles
  //   var osm = new L.TileLayer(osmUrl, {
  //     minZoom: 8,
  //     maxZoom: 13,
  //     attribution: osmAttrib
  //   });

  //   // setView() method to center the initial map
  //   map.setView(new L.LatLng(42.32, -82.95), 11);
  //   map.addLayer(osm);
  // }

  // initmap();

  // initialize the map
  // var map = L.map('mapid').setView([42.32, -82.95], 11);          // Windsor
  // var map = L.map('mapid').setView([42.35, -71.08], 11);          // Boston
  var map = L.map('mapid').setView([39.911640, 116.401938], 12);  // Beijing
  // var map = L.map('mapid').setView([-41.2858, 174.78682], 14);       // Wellington

  // load a tile layer
  L.tileLayer('http://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png',
   {
     attribution: 'Map data © <a href="http://openstreetmap.org">OpenStreetMap</a> contributors',
     maxZoom: 17,
     minZoom: 6
   }).addTo(map);


  // // load GeoJSON from an external file
  // var geojsonFeature = [
  //   {
  //     "type": "Feature",
  //     "properties": {
  //         "name": "Coors Field",
  //         "amenity": "Baseball Stadium",
  //         "popupContent": "This is where the Rockies play!"
  //     },
  //     "geometry": {
  //         "type": "Point",
  //         "coordinates": [-82.95, 42.32]       // opposite
  //     }
  //   },
  //   {
  //     "type": "Feature",
  //     "properties": {
  //         "name": "Coors Field",
  //         "amenity": "Baseball Stadium",
  //         "popupContent": "This is where the Rockies play2!"
  //     },
  //     "geometry": {
  //         "type": "Point",
  //         "coordinates": [-82.98, 42.30]       // opposite
  //     }
  //   },

  // ];
  // L.geoJson(geojsonFeature).addTo(map);

  /** - - - - - - - Numbered Markers - - - - - - - - - - - - - - - - - - - - - - - -  */
  /**
   * Numbered Markers in Leaflet (JS Mapping)
   * https://gist.github.com/comp615/2288108
   */
  L.NumberedDivIcon = L.Icon.extend({
    options: {
      // iconUrl: '<%= image_path("leaflet/marker_hole.png") %>',
      iconUrl: basePathUrl + 'sites/all/modules/custom/jobboard/image/marker_hole.png',
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
  /** - - - - - - - Numbered Markers - - - - - - - - - - - - - - - - - - - - - - - -  */

  var windsorMarker1 = new L.Marker([42.33, -82.95], {
    icon: new L.NumberedDivIcon({number: '2'})
  }).addTo(map);

  var beijingMarker1 = new L.Marker([39.8680001534,116.4951235621], {
    icon: new L.NumberedDivIcon({number: '1'})
  }).addTo(map);


  /**
   *
   1. 北京欢乐谷
   2. 北京海碗居老北京炸酱面(松榆里店)
   3. 北京宏源南门涮肉 （天坛南门）
   4. 和平门全聚德
   5. 江边城外烤全鱼(交大店) （北京动物园北门外）
   6. 姚记炒肝店（鼓楼包子）
   7. 北京簋街
   */

  // var jsonUrl = basePathUrl + "jsoninfo/jobboard/jobs/geomap.geojson";
  // jQuery.getJSON(jsonUrl, function(data){
  //   console.log(data);
  //   // add GeoJSON layer to the map once the file is loaded
  //   L.geoJson(geojsonFeature).addTo(map);
  // });

});
