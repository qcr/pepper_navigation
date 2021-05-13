# Sample data for testing

## offlinetest/
**Contains data for offline testing (mapping and localisation).**  
  
1. *[offlinetest/imgs](https://github.com/qcr/pepper_navigation/tree/main/data/offlinetest/imgs)* => contains image sequence obtained from the Pepper robot.  
2. *[offlinetest/kfls](https://github.com/qcr/pepper_navigation/tree/main/data/offlinetest/kfls)* => contains reference images obtained from *[select_ReferenceImages.m](https://github.com/qcr/pepper_navigation/blob/main/mapping/select_ReferenceImages.m)* located in the [*../mapping*](https://github.com/qcr/pepper_navigation/tree/main/mapping) folder.


## tmap/ 

**Contains topological map required for navigation.**  
  
*[tmap/conf.txt](https://github.com/qcr/pepper_navigation/blob/main/data/tmap/conf.txt)* => configuration file that defines the topological map.   
This file is generated from *[generate_configfile.m](https://github.com/qcr/pepper_navigation/blob/main/mapping/generate_configfile.m)* located in the [*../mapping*](https://github.com/qcr/pepper_navigation/tree/main/mapping) folder.
