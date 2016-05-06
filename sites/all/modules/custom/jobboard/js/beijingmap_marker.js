jQuery(document).ready(function(){

  var basePathUrl = Drupal.settings.basePath;
  var map;
  var ajaxRequest;
  var plotlist;
  var plotlayers=[];

  // initialize the map
  var map = L.map('mapid').setView([39.911640, 116.401938], 12);  // Beijing

  // load a tile layer
  L.tileLayer('http://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png',
   {
     attribution: 'Map data © <a href="http://openstreetmap.org">OpenStreetMap</a> contributors',
     maxZoom: 17,
     minZoom: 6
   }).addTo(map);

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
  var beijingMarker1 = new L.Marker([39.8680001534,116.4951235621], {
    icon: new L.NumberedDivIcon({number: '1'})
  }).addTo(map).bindPopup("北京欢乐谷");
  var beijingMarker2 = new L.Marker([39.8697123562,116.4689510129], {
    icon: new L.NumberedDivIcon({number: '2'})
  }).addTo(map).bindPopup("北京海碗居老北京炸酱面(松榆里店)");
  var beijingMarker3 = new L.Marker([39.8724295509,116.4073786290], {
    icon: new L.NumberedDivIcon({number: '3'})
  }).addTo(map).bindPopup("北京宏源南门涮肉 （天坛南门）");
  var beijingMarker4 = new L.Marker([39.8977622583,116.3795603460], {
    icon: new L.NumberedDivIcon({number: '4'})
  }).addTo(map).bindPopup("和平门全聚德");
  var beijingMarker5 = new L.Marker([39.9490800894,116.3429831306], {
    icon: new L.NumberedDivIcon({number: '5'})
  }).addTo(map).bindPopup("江边城外烤全鱼(交大店) （北京动物园北门外）");
  var beijingMarker6 = new L.Marker([39.9394981960,116.3905858926], {
    icon: new L.NumberedDivIcon({number: '6'})
  }).addTo(map).bindPopup("姚记炒肝店（鼓楼包子）");
  var beijingMarker7 = new L.Marker([39.939233, 116.422959], {
    icon: new L.NumberedDivIcon({number: '7'})
  }).addTo(map).bindPopup("北京簋街");

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

});
