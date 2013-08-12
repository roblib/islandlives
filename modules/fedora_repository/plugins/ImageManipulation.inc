<?php
// $Id$

/*
 *
 *
 * This Class implements the methods defined in the STANDARD_IMAGE content model
 */

class ImageManipulation {
  function ImageManipulation() {

  }

  function createPreview($parameterArray, $dsid, $file, $file_ext) {
    $system = getenv('System');
    $file_suffix = '_'. $dsid . '.'. $file_ext;
    $height = $parameterArray['height'];
    $width = $parameterArray['width'];
    $returnValue=TRUE;
    system("convert -resize $width -quality 85  \"$file\" -strip \"$file$file_suffix\" 2>&1 &", $returnValue);
    
    if ($returnValue == '0') {
      $_SESSION['fedora_ingest_files']["$dsid"] = $file . $file_suffix;
      return TRUE;
    }
    else {
      return $returnValue;
    }
  }

  function createPNG($parameterArray = NULL, $dsid, $file, $file_ext) {
    $file_suffix = '_'. $dsid . '.'. $file_ext;
    $returnValue = TRUE;
    system("convert  \"$file\"  \"$file$file_suffix\" 2>&1 &", $returnValue);

    if ($returnValue == '0') {
      $_SESSION['fedora_ingest_files']["$dsid"] = $file . $file_suffix;
      return TRUE;
    }
    else {
      return $returnValue;
    }
  }
  
  function createJP2($parameterArray = NULL, $dsid, $file, $file_ext ) {
    $file_suffix = "_$dsid.$file_ext";
    $return_value = TRUE;

    system('kdu_compress -i "'. $file . '" -o "'. $file . $file_suffix . '" -rate 0.5 Clayers=1 Clevels=7 "Cprecincts={256,256},{256,256},{256,256},{128,128},{128,128},{64,64},{64,64},{32,32},{16,16}" "Corder=RPCL" "ORGgen_plt=yes" "ORGtparts=R" "Cblk={32,32}" Cuse_sop=yes >/dev/null 2>&1', $returnValue);
    if ($returnValue == '0') {
      $_SESSION['fedora_ingest_files'][$dsid] = $file . $file_suffix;
    }
    system('kdu_compress -i "'. $file . '" -o "'. $file . "_lossless.jp2".'" -rate -,0.5 Clayers=2 Creversible=yes Clevels=8 "Cprecincts={256,256},{256,256},{128,128}" Corder="RPCL" ORGgen_plt="yes" ORGtparts="R" Cblk="{32,32}" >/dev/null 2>&1', $returnValue);
    if ($returnValue == '0') {
      $_SESSION['fedora_ingest_files']["LOSSLESS_JP2"] = $file . '_lossless.jp2';
    }
    system("convert  -resize 800 \"$file\"  \"$file-med.jpg\" >/dev/null 2>&1", $returnValue);
    if ($returnValue == '0') {
      $_SESSION['fedora_ingest_files']["JPG"] = $file . '-med.jpg';
    }
    system("convert \"$file\" -thumbnail x2000 -thumbnail x450  -resize '450x<' -resize 50% -fuzz 1% -trim +repage -gravity center -crop 200x200+0+0 +repage -format jpg -quality 100  \"$file-tn.jpg\" >/dev/null 2>&1", $returnValue);
    if ($returnValue == '0') {
      $_SESSION['fedora_ingest_files']["TN"] = $file . '-tn.jpg';
    }
    $output=array();
    
    if ($returnValue == '0') {
      return TRUE;
    }
    else {
      return $returnValue;
    }
  }

  //use imagemapi to manipulate images instead of going directly to imagemagick or whatever
  function manipulateImage($parameterArray = NULL, $dsid, $file, $file_ext ) {
    $height=$parameterArray['height'];
    $width=$parameterArray['width'];

    $file_suffix = '_'. $dsid . '.' . $file_ext;
    $returnValue = TRUE;

    $image = imageapi_image_open($file);

    if (!$image) {
      drupal_set_message(t("Error opening image."));
      return FALSE;
    }

    if (!empty($height) || !empty($width)) {
      $returnValue = imageapi_image_scale($image, $height, $width);
    }

    if (!$returnValue) {
      drupal_set_message(t("Error scaling image."));
      return $returnValue;
    }
    $filename = substr(strrchr($file, '/'), 1);
    $output_path = $_SERVER['DOCUMENT_ROOT'] . base_path() . file_directory_path() . '/'. $filename . $file_suffix;
    $returnValue = imageapi_image_close($image, $output_path);
    if ($returnValue) {
      $_SESSION['fedora_ingest_files']["$dsid"] = $file . $file_suffix;
      return TRUE;
    }
    else {
      return $returnValue;
    }
  }

  function createThumbnailFromPDF($parameterArray, $dsid, $file, $file_ext) {
    $height=$parameterArray['height'];
    $width=$parameterArray['width'];
    $file_suffix = '_'. $dsid . '.'. $file_ext;
    $returnValue = TRUE;
    //system("convert $file\[0\] -thumbnail 128x128 $uploaddir$thumb");
    // Use this for Linux.
    if (stristr( $_SERVER[ 'SERVER_SOFTWARE'], 'microsoft')) {
    } 
    elseif (stristr($_SERVER['SERVER_SOFTWARE'], 'linux')) {
        $cmdline = "/usr/local/bin/convert \"$file\"\[0\] -colorspace RGB -thumbnail $width". "x$height \"$file$file_suffix\"";
      } 
      elseif (stristr($_SERVER['SERVER_SOFTWARE'], 'unix')) {
        // Use this for Mac OS X (MAMP)
          $cmdline = "sips -s format jpeg \"$file\" -z $height $height --out \"$file$file_suffix\" >/dev/null";
        } 
        else {
          $cmdline = "convert \"$file\"\[0\] -colorspace RGB -thumbnail ". $width ."x". $height . " \"$file$file_suffix\"";
        }

    system($cmdline, $returnValue);
    //system("convert $file\[0\] -thumbnail 128x128 $uploaddir$thumb");
    $var = $file . $file_suffix . ' returnvalue= ' . $returnValue;

    if ($returnValue == '0') {
      $_SESSION['fedora_ingest_files']["$dsid"] = $file . $file_suffix;
      return TRUE;
    }
    else {
      return $returnValue;
    }
  }

  function createThumbnail($parameterArray, $dsid, $file, $file_ext) {
    // var_dump($parameterArray);exit(0);
    $file_suffix = '_'. $dsid . '.'. $file_ext;
    $height=$parameterArray['height'];
    $width=$parameterArray['width'];
    $returnValue = TRUE;
    system("convert -resize $width -quality 85  \"$file\" -strip \"$file$file_suffix\" 2>&1 &", $returnValue);
    //system("convert -resize $width -quality 85  \"$file\" -strip \"$file$file_suffix\"",$returnValue);

    if ($returnValue == '0') {
      $_SESSION['fedora_ingest_files']["$dsid"] = $file . $file_suffix;
      return TRUE;
    }
    else {
      return $returnValue;
    }

  }
}
