<?php
/**
 * class
   $JsonDataCacheFile = new JsonDataCacheFile();
   $JsonDataCacheFile->termCity();
 *
 */
class JsonDataCacheFile {

  /** - - - - - Term Json- - - - - - - - - - - - - - - */
  /**
   * generate json cache file
   */
  public function termCity() {
    $file = 'cache/term_city_basic.json';
    $JsonDataUnit = new JsonDataUnit();
    OtherCommonJson::saveDataToJson($JsonDataUnit->termCity(TRUE), $file);
  }
}
