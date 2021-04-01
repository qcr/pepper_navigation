/* 
This file is part of the BIAS library (Basic ImageAlgorithmS).

Copyright (C) 2003-2009    (see file CONTACT for details)
  Multimediale Systeme der Informationsverarbeitung
  Institut fuer Informatik
  Christian-Albrechts-Universitaet Kiel


BIAS is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or
(at your option) any later version.

BIAS is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with BIAS; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/


#ifndef __METADATA_HH__
#define __METADATA_HH__
#include "bias_config.h" 


#include <vector>
#include <iostream>
#include <string>

namespace BIAS {

  /** @class AppData
      @brief this is a chunk of metadata, also see MetaData 
      
      AppData can hold the meta datum either in ascii format (in this case 
      AppData.tag is set to AppData::MD_USE_ASCII) or in binary format (in this
      case AppData.tag is set to anything except AppData::MD_USE_ASCII or 
      AppData::MD_Invalid)

      Basic operators for reading, writing and assignig exist

      @author woelk 11/2003 */

  class BIASImageBase_EXPORT AppData
  {
  public:
    /** @enum TAppData 
        @brief Type of AppData 
        
        If set to MD_USE_ASCII, AppData.tag, AppData.length and AppData.data 
        are invalid. AppData.stag and AppData.sdata should be used.
        
        If set to anything else except MD_Invalid, AppData.tag, AppData.length 
        and AppData.data are valid. AppData.stag and AppData.sdata should not 
        be used in this case. 
        
        The string tag AppData.stag can be generated from the enum id by 
        removing 'MD_', substituting underscores with blanks and bracketing 
        with '#[' and ']'. 
        
        For example tag=MD_Inertial_Sensor evolves to 
        stag="#[Inertial Sensor]".

        !! MD_ASCII_DATA is deprecated and should no longer be used. !! 
        It is keept her for compatibility reasons.
    */
    enum TAppData {MD_PMatrix=0, MD_Orientation=1, MD_TimeStamp=2, 
                   MD_PTU_Data=3, MD_Inertial_Sensor=4, MD_ASCII_DATA=5, 
                   MD_RMatrix=6, MD_KMatrix=7, MD_CVector=8, MD_HomgPoint2D=9, 
                   MD_Invalid=10, MD_UUID=11, MD_USE_ASCII=12, 
                   MD_Focal_Length=14, MD_GPS_Data=15, MD_Projection = 16, 
		   MD_RefUUID=17, MD_PMD_DataDesc=18};
  public:
    AppData();
    ~AppData();

    AppData(const AppData& app);

    AppData& operator=(const AppData& app);

    /** @brief Resets all members. */
    void Clear();

    /** checks if stag and sdata have the correct format */
    int CheckFormat();

    /** @brief The tag defines the type of data, e.g. MD_PMatrix
        is set to MD_USE_ASCII if ascii data is used. See TAppData. */
    enum TAppData tag;
    /// number of bytes used by the data block
    int length;
    /// pointer to block of data
    char *data;
    /// the tag as given in ascii meta data
    std::string stag;
    /// the data as given in ascii meta data
    std::string sdata;
  };

  /** @relates AppData */
  std::ostream& operator<<(std::ostream& os, const AppData& ad);
  /** @relates AppData */
  std::istream& operator>>(std::istream& is, AppData& ta);
  
  /** @relates TAppData */
  std::ostream& operator<<(std::ostream& os, const AppData::TAppData& ta);

  
  /** @class MetaData
      @brief this class collects all additional data chunks of type AppData
      to be written into/read from an image file 

      See ExampleMetaData for usage.

      @author woelk 11/2003 */
  class BIASImageBase_EXPORT MetaData : public std::vector<AppData>
  {
  public:
    MetaData();

    MetaData(const MetaData& M) {
      (*this) = M;
    }

    MetaData& operator=(const MetaData& M) {
      std::vector<AppData>::operator=(M);
      return (*this);
    }

    ~MetaData();

    /** @brief Add a new binary meta datum.
        
        Replace old binary meta datum if already present. 
        Does not check if corresponding ascii datum is present. */
    void Add(enum AppData::TAppData ID, unsigned int length, char *data);
    void Add(AppData& ad);
    /** @brief Add a new ascii meta datum

        Replace old ascii meta datum if already present.
        Does not check if corresponding binary datum is present. */
    void Add(const std::string &tag, const std::string& data);

    /** @brief searches for tag in binary coded AppDatas.

        returns:   - position in vector if tag is found

                   - -2 if tag not found but ascii data present

                   - -1 if tag not found and no ascii data present */
    int Find(const enum AppData::TAppData tag, AppData &data) const;
    /** @brief searches for tag in ascii coded AppDatas.

        returns:   - position in vector if tag is found

                   - -2 if tag not found but binary data present

                   - -1 if tag not found and no binary data present */
    int Find(const std::string &tag, AppData &data) const;
    /** @brief cascades Find(enum AppData::TAppData tag, AppData &data) and
        Find(std::string tag, AppData &data) */
    int Find(const enum AppData::TAppData tag, const std::string &stag, 
             AppData &data) const;

    /** @brief Delete the meta datum identified by tag.
        Does not search for the corresponding ascii meta datum. */
    void Delete(enum AppData::TAppData tag);
    /** @brief Delete the meta datum identified by tag.
        Does not search for the corresponding binary meta datum. */
    void Delete(const std::string &tag);

    /** @brief Writes only the meta datas where AppData::tag!=MD_USE_ASCII 
        to os. */
    std::ostream &WriteBinary(std::ostream& os) const;
    /** @brief Writes only the meta datas where AppData::tag==MD_USE_ASCII 
        to os. */
    std::ostream &WriteAscii(std::ostream& os) const;

    /// @brief dumps this to cout, unfinished piece of code
    void Dump(std::ostream& os=std::cout);
  };
  
  /** @relates MetaData 
      @brief Reads from is. Decides whether to read ascii format or binary 
      format */
   BIASImageBase_EXPORT std::istream& operator>>(std::istream& is, MetaData& md);
  /** @relates MetaData 
      @brief Dumb write operator writes everything to os.
      !!! binary and ascii data are written mixed, if both are present !!! */
   BIASImageBase_EXPORT std::ostream& operator<<(std::ostream& is, const MetaData& md);
  
} //namespace




#endif
