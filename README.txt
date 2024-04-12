*** Description ***
This program converts an exported ".csv" file of text nodes from Microstation 
to their combined total lengths of bore and plow fore each road.

*** Usage **
1. Obtain ".csv" file.
Extract the text nodes from Microstation using the reports feature.
This will extract them into a ".xlsx" file. You must convert that
file into a ".csv" file. You can easily do that by opening it in Excel
and using the "save as" feature.

2. Put the extracted file in the input_files directory
Take the newly created ".csv" file and put it in the "input_files" folder.
If there is no folder present, run the "Converter.exe" file to create one.

3. Convert
Run the "Converter.exe" file to convert the file. If everything worked properly,
a new text file should appear in the "output_files" folder. That will contain
the bore and plow lengths.

*** Optional Settings ***
You can change the cabel type in the "cabel_type.txt" file.
Simply change the first line to the text you want after the bore and plow numbers.