ofxSurfingHelpers
=============================

# IMPORTANT

Currently using improved and simple classes from here: https://github.com/moebiussurfing/ofxSurfingHelpersLite

# OVERVIEW

[SCREENSHOTS](Examples/README.md)  

A ( **WIP** ) personal collection of some useful stuff for **openFrameworks**:  

OF-Utils / Classes / Snippets / Helpers / Serializers / Rectangles / Interactive objects / OF Native UI Widgets and many ImGui / ofxSurfingImGui modules ...

## FEATURES

Look to the available classes in **OF_ADDON/src/** for details.

<details>
  <summary>FILES LIST</summary>
  <p>

\ofxSurfingHelpers.h  
\ofxSurfingConstants.h  

\devices\SurfingWebcam.cpp  
\devices\SurfingWebcam.h  

\gui\ofxSurfing_ofxGui.h  
\gui\ofxSurfing_Widgets.h  
\gui\notifiers\surfingNotify.cpp  
\gui\notifiers\surfingNotify.h  
\gui\widgets\BarValue.h  
\gui\widgets\BpmTapTempo.h  
\gui\widgets\CircleBeat.h  
\gui\widgets\CircleCycle.h  
\gui\widgets\RectBeat.h  
\gui\widgets\TextBoxWidget.h  

\utils\ofxAutosaveGroupTimer.cpp  
\utils\ofxAutosaveGroupTimer.h  
\utils\ofxMouseRuler.cpp  
\utils\ofxMouseRuler.h  
\utils\surfingDrawPlanes.h  
\utils\surfingEasyCam.h  
\utils\surfingFiles.h  
\utils\surfingGlmSplit.h  
\utils\surfingMaths.h  
\utils\surfingMouseHandler.h  
\utils\surfingParamIndexToggles.h  
\utils\surfingParamNorm.h  
\utils\surfingParamsRandom.h  
\utils\surfingSceneTesters.h  
\utils\surfingSounds.h  
\utils\surfingTimers.h  

\ImGui\ofxSurfingPresetsLite.h  
\ImGui\surfingEasyCamImGui.h  
\ImGui\surfingScene3dRules.h  

  </p>
</details>


<details>
  <summary>EXAMPLES</summary>
  <p>
 
### PresetsLite 
Presets from an ofParmeterGroup.  

### ParamsIndexToggles 
Matrix toggles from an ofParameter<int> index.  

### MouseHandler
Auto hides the mouse using a timer and shows a pointer rule to show mouse coordinates and measure distances.  

### ParamsAutoSave
Queue many ofParameterGroup's and they will be auto-saved using a timer. 
Also on exit and reloaded when starting the app. 
That allows your settings saved even when your app crashes, even if you usually make the saving only on exit.

### Scene3dRules
Custom colors and grid plane.  

</p>
</details>
<br>

<details>
  <summary>THIRD PARTY ADD-ONS</summary>
  <p>

### Bundled into **OF_ADDON/libs/**
Already added to `OFX_ADDON/libs/`.  
No need to clone nor add to **PROJECT GENERATOR****.  
* [ofxScaleDragRect](https://github.com/moebiussurfing/ofxScaleDragRect) / _FORK_ from [roymacdonald](https://github.com/roymacdonald/ofxScaleDragRect)  
* [ofxMSAInteractiveObject](https://github.com/moebiussurfing/ofxMSAInteractiveObject) / _FORK_ from [memo](https://github.com/memo/ofxMSAInteractiveObject)  
* [ofxSerializer](https://github.com/bakercp/ofxSerializer)
  </p>
</details>

<details>
  <summary>OTHER REFERENCES / RESOURCES</summary>
  <p>

https://github.com/FilikaDesign/ofxFilika   
https://github.com/alptugan/ofxFilikaUtils  
https://github.com/companje/ofxExtras/blob/master/src/ofxExtras.cpp  
https://github.com/chimanaco/ofxUtils/tree/master/src  
https://github.com/nightshining/ofxControlUtils  
https://github.com/bernardgeyer/ofxhUtils  
https://github.com/NickHardeman/Tools  
https://github.com/bakercp/ofxUtils  
https://github.com/Iwanaka/ofxMyUtil  
https://github.com/perevalovds/ofxKu/tree/master/src  
https://github.com/ofTheo/ofxFileHelper  

_Some of the bundled classes here may have been taken from the above repositories. Thanks to their respective coders!_
  </p>
</details>

<details>
  <summary>TESTED SYSTEMS</summary>
  <p>

  - **Windows 10** / **VS 2022** / **OF ~0.11**
  </p>
</details>

## AUTHOR
An add-on by **@moebiusSurfing**  
*( ManuMolina ) 2019-2023*  

## CONTACT / TUNE
<p>
<a href="mailto:moebiussurfing@gmail.com" target="_blank">Email</a> |
<a href="https://twitter.com/moebiusSurfing/" rel="nofollow">Twitter</a> | 
<a href="https://www.youtube.com/moebiusSurfing" rel="nofollow">YouTube</a> | 
<a href="https://www.instagram.com/moebiusSurfing/" rel="nofollow">Instagram</a> 
</p>

## LICENSE
MIT License
