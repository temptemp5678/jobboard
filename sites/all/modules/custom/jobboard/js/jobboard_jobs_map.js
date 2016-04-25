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
   var map = L.map('mapid').setView([42.32, -82.95], 11);

   // load a tile layer
   L.tileLayer('http://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png',
     {
       attribution: 'Map data © <a href="http://openstreetmap.org">OpenStreetMap</a> contributors',
       maxZoom: 17,
       minZoom: 6
     }).addTo(map);


   // load GeoJSON from an external file
   var geojsonFeature = [
     {
       "type": "Feature",
       "properties": {
           "name": "Coors Field",
           "amenity": "Baseball Stadium",
           "popupContent": "This is where the Rockies play!"
       },
       "geometry": {
           "type": "Point",
           "coordinates": [-82.95, 42.32]       // opposite
       }
     },
     {
       "type": "Feature",
       "properties": {
           "name": "Coors Field",
           "amenity": "Baseball Stadium",
           "popupContent": "This is where the Rockies play!"
       },
       "geometry": {
           "type": "Point",
           "coordinates": [-82.98, 42.30]       // opposite
       }
     },

   ];
   L.geoJson(geojsonFeature).addTo(map);

   // var jsonUrl = basePathUrl + "jsoninfo/jobboard/jobs/geomap.geojson";
   // jQuery.getJSON(jsonUrl, function(data){
   //   console.log(data);
   //   // add GeoJSON layer to the map once the file is loaded
   //   L.geoJson(geojsonFeature).addTo(map);
   // });

});
