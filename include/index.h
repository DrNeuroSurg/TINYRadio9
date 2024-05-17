/*
 *  index.h
 *
 *  Created on: 10.05.2024
 *  Updated on: 11.05.2024
 *      Author: DrNeurosurg
 *
 *  successfully tested with Chrome and Firefox and Safari
 *
 */

#ifndef INDEX_H_
#define INDEX_H_

#include "Arduino.h"

// file in raw data format for PROGMEM
//
const char index_partOne[] PROGMEM = R"=====(

<!DOCTYPE html>
<html>
  <head>
    <meta charset="utf-8" />
    <title>Stations Editor</title>
    <script src="https://cdn.jsdelivr.net/npm/@json-editor/json-editor@latest/dist/jsoneditor.min.js"></script>

 <style type="text/css">
       
       .c,body{
           text-align:left;
           font-family: 'Courier New', monospace;
           border:none;
       }
       div,input,select {
           padding:5px;
           font-size:1em;
           margin:5px 0;
           box-sizing:border-box
       }
       
       input,button,select,.msg{
           border-radius:.3rem;
           width: 100%
           font-family: 'Courier New', monospace;
           margin-bottom: 1px;
           margin-right: 10px;
       }
       input[type=radio],input[type=checkbox]
       {
           width:auto
       }
       button,input[type='button'],input[type='submit']{
           cursor:pointer;
           border:0;
           background-color:#1fa3ec;
           color:#fff;
           line-height:2.4rem;
           font-size:1.2rem;
           width:100%
           font-family: 'Courier New', monospace;
       }
       input[type='file']{
           border:1px solid #1fa3ec
       }
       .wrap {
           text-align:left;
           display:inline-block;
           min-width:260px;
           max-width:500px
       }
       // links
       a{
           color:#000;
           font-weight:700;
           text-decoration:none
       }
       a:hover{
           color:#1fa3ec;
           text-decoration:underline
       }
     // quality icons
     .q{
         height:16px;
         margin:0;
         padding:05px;
         text-align:right
         ;min-width:38px;
         float:right
        }
       .q.q-0:after{
           background-position-x:0
       }
       .q.q-1:after{
           background-position-x:-16px
       }
       .q.q-2:after{
           background-position-x:-32px
       }
       .q.q-3:after{
           background-position-x:-48px
       }
       .q.q-4:after{
           background-position-x:-64px
       }
       .q.l:before{
           background-position-x:-80px;
           padding-right:5px
       }
       .ql .q{
           float:left
       }
       .q:after,.q:before{
           content:'';
           width:16px;
           height:16px;
           display:inline-block;
           background-repeat:no-repeat;
           background-position: 16px 0;
           background-image:url('data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAGAAAAAQCAMAAADeZIrLAAAAJFBMVEX///8AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAADHJj5lAAAAC3RSTlMAIjN3iJmqu8zd7vF8pzcAAABsSURBVHja7Y1BCsAwCASNSVo3/v+/BUEiXnIoXkoX5jAQMxTHzK9cVSnvDxwD8bFx8PhZ9q8FmghXBhqA1faxk92PsxvRc2CCCFdhQCbRkLoAQ3q/wWUBqG35ZxtVzW4Ed6LngPyBU2CobdIDQ5oPWI5nCUwAAAAASUVORK5CYII=');}
     // icons @2x media query (32px rescaled)
     @media (-webkit-min-device-pixel-ratio: 2),(min-resolution: 192dpi){.q:before,.q:after {
         background-image:url('data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAALwAAAAgCAMAAACfM+KhAAAALVBMVEX///8AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAADAOrOgAAAADnRSTlMAESIzRGZ3iJmqu8zd7gKjCLQAAACmSURBVHgB7dDBCoMwEEXRmKlVY3L//3NLhyzqIqSUggy8uxnhCR5Mo8xLt+14aZ7wwgsvvPA/ofv9+44334UXXngvb6XsFhO/VoC2RsSv9J7x8BnYLW+AjT56ud/uePMdb7IP8Bsc/e7h8Cfk912ghsNXWPpDC4hvN+D1560A1QPORyh84VKLjjdvfPFm++i9EWq0348XXnjhhT+4dIbCW+WjZim9AKk4UZMnnCEuAAAAAElFTkSuQmCC');
         background-size: 95px 16px;}
     }
     // msg callouts
     .msg{
         padding:20px;
         margin:20px 0;
         border:1px solid #eee;
         border-left-width:5px;
         border-left-color:#777
     }
       .msg h4{
           margin-top:0;
           margin-bottom:5px
       }
       .msg.P{
           border-left-color:#1fa3ec
       }
       .msg.P h4{
           color:#1fa3ec
       }
       .msg.D{
           border-left-color:#dc3630
       }
       .msg.D h4{
           color:#dc3630
       }
       .msg.S{
           border-left-color: #5cb85c
       }
       .msg.S h4{
           color: #5cb85c
       }
     // lists
     dt{
         font-weight:bold
     }
       dd{
           margin:0;
           padding:0 0 0.5em 0;
           min-height:12px
       }

       .h{
           display:none
       }
       button{
           transition: 0s opacity;
           transition-delay: 0.5s;
           transition-duration: 0.5s;
           cursor: pointer
           font-family: 'Courier New', monospace;
       }
       button.D{
           background-color:#dc3630
       }
       button:active{
           opacity:50% !important;
           cursor:wait;
           transition-delay: 0s
       }
     // invert
     body.invert,body.invert a,body.invert h1 {
         background-color:#060606;
         color:#fff;
     }
       body.invert .msg{
           color:#fff;
           background-color:#282828;
           border-top:1px solid #555;
           border-right:1px solid #555;
           border-bottom:1px solid #555;
       }
       body.invert .q[role=img]{
           -webkit-filter:invert(1);
           filter:invert(1);
       }
       :disabled {
           opacity: 0.5;
       }
     
     
     body {
      margin: 1em;
    }

    h1 {
      text-align: center;
      font-size: 3rem;
    }
      h2 {
      text-align: center;
    }
    .meine {
        position: absolute;
        left: 50%;
        transform: translateX(-50%);
    }
       
   input[type="color"] {
       -webkit-appearance: none;
       border: none;
       width: 100%;
       height: 30px;
       border-radius: 1px;

   }
   input[type="color"]::-webkit-color-swatch-wrapper {
       padding: 0;
       border-radius: 1px;
   }
   input[type="color"]::-webkit-color-swatch {
       border: solid 1px #999;
       border-radius: 1px;
   }
   
   body {
    background-color:  #e6e6e6;
    font-family: 'Courier New', monospace;
   }
       
 .row {
     align-items: center;
     background-color:  #e6e6e6;
     box-shadow:
       12px 12px 16px 0 rgba(0, 0, 0, 0.25),
       -8px -8px 12px 0 rgba(255, 255, 255, 0.55);
     border-radius: 5px;
     margin-bottom: 10px;
 }
       
      .je-indented-panel {
          all: none;
          border-left-color:rgb(255,0,0);
       }

  .je-header {
      margin-top: 1px;
      margin-bottom: 5px;
      font-weight: bold;
      font-size: 1.5rem;
      line-height: 1.5rem;
  }

  .je-form-input-label {
      padding-bottom: 5px;
      font-size: 1.5rem;
  }

       label {

           font-size: 1.5rem;
           font-weight: normal;
       }
        .control-group {
            font-weight: bold;
            font-size: 1.5rem;
        }

       .je-table tr td:first-child {
               width:25%;
       }

       .je-table tr td:first-child + td {
                   width:50%;
           }
            
        input[type='text']{
            width:100%;
        }
            

    .json-editor-btn-delete, .json-editor-btn-moveup , .json-editor-btn-movedown,.json-editor-btn-add,.json-editor-btntype-deletelast, input[type='text']{
           font-family: 'Courier New', monospace;
       }

    tr {
            font-weight: bold;
            font-size: 1.5rem;
        }
       
    th {
        text-align: center;
    }

       input[type="button"] {
           font-family: 'Courier New', monospace;
           font-size: 1.5rem;
           width: 100%;
           }

  </style>
  </head>
  
  <body>
    <h1><br>T.I.N.Y.Radio</h1>
    <h2>(This Is Not yoRadio)</h2>

    <div id='editor_holder'></div>
   
   <div class ="meine">
    <p>
          <input type="button" value="SPEICHERN & UPDATE" onclick="javascript:postIt('/save', editor.getValue());"/>
    </p>
   </div>
   
   <script>
        
  )=====";

 const char index_partTwo[] PROGMEM = R"=====(

 JSONEditor.defaults.language = "de";
        JSONEditor.defaults.languages.de = {
            button_add_row_title: 'Neue {{0}} hinzufügen',
            button_move_down_title: 'nach unten',
            button_move_up_title: 'nach oben',
            button_delete_last_title: 'Letze {{0}} löschen',
            button_delete_all_title: 'Alle löschen',
            button_delete_row_title: '{{0}} löschen',
            button_delete_row_title_short: 'Löschen',
            button_delete_node_warning: 'Sind sie sicher ?',
        }
        
      // Initialize the editor with a JSON schema
      var editor = new JSONEditor(document.getElementById('editor_holder'),{

        schema: {
            "title": " ",
            "type": "object",
            "required": [
              "stations"
            ],
            "properties": {
              "colorBackground": {
                  "title": "Hintergrund",
                  "type": "string",
                  "format": "color",
                  "default": "#00000"
                },
              "colorStation": {
                  "title": "Station",
                  "type": "string",
                  "format": "color",
                  "default": "#ffa500"
                },
            "colorMarker": {
                  "title": "Marker",
                  "type": "string",
                  "format": "color",
                  "default": "#caf0fe"
                },
            "colorTuning": {
                  "title": "Zeiger Sender",
                  "type": "string",
                  "format": "color",
                  "default": "#e32400"
                },
            "colorVolume": {
                  "title": "Zeiger Lautstärke",
                  "type": "string",
                  "format": "color",
                  "default": "#6aff12"
                },
              "stations": {
                "title": "Stationen",
                "type": "array",
                "format": "table",
                "uniqueItems": false,
                "items": {
                  "type": "object",
                  "title": "Station",
                  "properties": {
                    "shortName": {
                      "title": "Name (kurz)",
                      "type": "string"
                    },
                    "StreamURL": {
                      "title": "StreamURL",
                      "type": "string"
                    }
                  }
                }
              }
            }
          },
        disable_properties: "true",
        disable_collapse: "true",
        disable_edit_json: "true",
        show_errors: "never"
        });
        
        editor.on('ready', function() {
          editor.setValue(jsondata);
        })

        
  
      function postIt(path, params, method) {
          method = method || "post"; // Set method to post by default if not specified.

          var jString = JSON.stringify(params);
          console.log(jString);
          var form = document.createElement("form");
          form.setAttribute("method", method);
          form.setAttribute("action", path);

          var hiddenField = document.createElement("input");
          hiddenField.setAttribute("type", "hidden");
          hiddenField.setAttribute("name", "Settings");
          hiddenField.setAttribute("value", jString);
          form.appendChild(hiddenField);
          
          document.body.appendChild(form);
          form.submit();

        }
    </script>
  </body>
</html>
  )====="; 

// DEFAULTS
const char index_PayLoad[] PROGMEM = R"=====(

        var jsondata = {
            colorBackground: "#00000",
            colorStation: "#ffa500",
            colorMarker: "#caf0fe",
            colorTuning: "#e32400",
            colorVolume: "#6aff12",
            stations: [
                {shortName: "COSTA.D.MAR", StreamURL: "http://radio4.cdm-radio.com:8020/stream-mp3-Chill_autodj"},
                {shortName: "Kiss.FM", StreamURL: "http://topradio-stream31.radiohost.de/kissfm_mp3-128"},
                {shortName: "sun radio", StreamURL: "http://radio2.vip-radios.fm:8068/stream-128kmp3-CafeSoulside_autodj"}
               ]};
)====="; 


#endif /* INDEX_H_ */
