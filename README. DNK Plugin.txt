Main files from the plug-in for generating objects with given coordinates in a text file and random height from a certain interval.

They implement the basic logic.

Working with the plugin was as follows:
1. Create / open a level.
2. The level must have a landscape [Landscape]
3. Click on the plugin icon in the Toolbar or select it in the [Window-> DNKPlugin] menu
4. Set the maximum and minimum height. Size in cm (default in UE4)
5. Enter the path to the text file with the type and coordinates of the object. You can use up to 5 types of objects.
6. Assign each object type used in the text file to a corresponding mesh in the ContentBrowser. To do this, right-click on the desired StaticMesh and select "CopyReference" from the context menu. Next, into the desired number of the object type, paste the copied link to StaticMesh in the appropriate field.
7. Click on the Generate button.
8. The log (Window-> DeveloperTools-> OutputLog) displays reference data regarding the process of generating objects.

The testCoordinates text file was written to test the plugin. Contains the type of objects (from 1 to 5) and their coordinates (X and Y) as required.

Best regards, Denis (DNK).