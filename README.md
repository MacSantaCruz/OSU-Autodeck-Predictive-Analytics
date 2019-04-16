# OSU-Autodeck-Predictive-Analytics
This project must increase productivity of a daily, experienced user of Autodesk Inventor. This is accomplished by evaluating individual user command usage, identifying command patterns from the data pool, recognize users command sequence and present recommended, next-command options in an easily-accessible UI

##Start up guide
  1. Install Autodesk Inventor Professional 2019
  2. pull master branch
  3. copy "Autodesk.CommandWhisperer.Inventor.addin" from "OSU-Autodeck-Predictive-Analytics\Add-In-Files\ConnectedCommandWhisperer\ludeksulc-commandwhisperer-ae756d52ad56\CommandWhisperer" to "C:\ProgramData\Autodesk\Inventor 2019\Addins"
  4. copy "CommandWhisperer.dll" from "OSU-Autodeck-Predictive-Analytics\Add-In-Files\ConnectedCommandWhisperer\ludeksulc-commandwhisperer-ae756d52ad56\x64\Debug" to "C:\Program Files\Autodesk\Inventor 2019\Bin"
  5. Start up Inventor 2019
  6. In the opening screen navigate to the "Tools" tab, click on the Add-Ins option. This will open the Add-In manager window.  Find "CommandWhispererAddInServer" in the menu.  Check that its load behavior is "Automatic / Loaded". If it is not, check the "Load Automatically" box and if the Addin is not loaded check the Loaded/Unloaded.
  7. Open a command console (VS Developer Command Prompt ,git bash, etc.). cd into "OSU-Autodeck-Predictive-Analytics\Add-In-Files\ConnectedCommandWhisperer\LSTM files\"
  8. run "python lmst.py"
  9. watch the command console, it should print "got client" if it successfully connects to Inventor
  10. Back in Inventor click the "Drawing tab" to open a new drawing.
  11. in the header (File, Place View, Annotate, etc.) click Add-Ins. You should Command Whisperer, click on the text.  Clicking that will open a dialog box.
  12. Click the header item "Sketch"
  13. Click any tool btn (line, Circle, Rectangle, etc.)
  14. the Dialog window should update, listing "commandName, PRED: predictedCommand"
  15. the console window running lmst.py will also update showing the commandName and predictedCommand
  16. Close Inventor FIRST before closing/CTRL+C-ing the lmst window.
