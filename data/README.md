# Sample data for testing
## [offlinetest](https://github.com/suuman/pepper_navigation/tree/main/data/offlinetest)
### Contains data for offline testing (mapping and localisation)
1. *[offlinetest/imgs](https://github.com/suuman/pepper_navigation/tree/main/data/offlinetest/imgs)* => conatins image sequence obtained from Pepper Robot
2. *[offlinetest/kfls](https://github.com/suuman/pepper_navigation/tree/main/data/offlinetest/kfls)* => conatins reference images obatined from *[../mapping/select_ReferenceImages.m](https://github.com/suuman/pepper_navigation/blob/main/mapping/select_ReferenceImages.m)* located in the [*../mapping*](https://github.com/suuman/pepper_navigation/tree/main/mapping) folder.


## [tmap](https://github.com/suuman/pepper_navigation/tree/main/data/tmap) 
### Contains topological map required for navigation
*[tmap/conf.txt](https://github.com/suuman/pepper_navigation/blob/main/data/tmap/conf.txt)* => configuration file that defines the topological map. This file is generated from *[../mapping/generate_configfile.m](https://github.com/suuman/pepper_navigation/blob/main/mapping/generate_configfile.m)* located in the [*../mapping*](https://github.com/suuman/pepper_navigation/tree/main/mapping) folder.
