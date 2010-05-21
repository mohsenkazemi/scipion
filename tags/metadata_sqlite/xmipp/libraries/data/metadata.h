/***************************************************************************
 * 
 * Authors:     J.R. Bilbao-Castro (jrbcast@ace.ual.es)
 *
 * Unidad de  Bioinformatica of Centro Nacional de Biotecnologia , CSIC
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 * 02111-1307  USA
 *
 *  All comments concerning this program package may be sent to the
 *  e-mail address 'xmipp@cnb.csic.es'
 ***************************************************************************/

#ifndef METADATA_H
#define METADATA_H

#include <map>
#include <vector>
#include <iostream>
#include <iterator>
#include <sstream>
#include "funcs.h"
#include "strings.h"
#include "metadata_sql.h"
#include <time.h>
#include <stdio.h>
#include <sstream>

//empty vector used for inizialization

/// @defgroup MetaData Metadata management
/// @ingroup DataLibrary

/// @defgroup MetaDataClass Metadata class management
/// @ingroup MetaData

/** MetaData Manager.
 * @ingroup MetaDataClass
 *
 * The MetaData class manages all procedures related to
 * metadata. MetaData is intended to group toghether old
 * Xmipp specific files like Docfiles, Selfiles, etc..
 * 
 */

//useful to init values to zero
static double zeroD=0.;
static double    oneD=1.;
static bool  falseb=false;

class MDQuery;

class MetaData
{
private:
    //std::map<long int, MetaDataContainer *> objects; ///< Effectively stores all metadata

    // Used by firstObject, nextObject and lastObject to keep a pointer
    // to the "active" object. This way when you call setValue without
    // an objectID, this one is chosen
    //std::map<long int, MetaDataContainer *>::iterator objectsIterator;

    // Allows a fast search for pairs where the value is
    // a string, i.e. looking for filenames which is quite
    // usual
    std::map<std::string, long int> fastStringSearch;
    MDLabel fastStringSearchLabel;

    std::string path; ///< A parameter stored on MetaData Files
    std::string comment; ///< A general comment for the MetaData file

    bool isColumnFormat; ///< Format for the file, column or row formatted

    /**Input file name
     * Where does this MetaData come from/go to be stored?
     */
    FileName inFile;

    /**Name of the Table in the DB
     */
    std::string tableName;

    /** What labels have been read from a docfile/metadata file
     *   and/or will be stored on a new metadata file when "save" is
     *   called
     **/
    std::vector<MDLabel> activeLabels;

    /** When reading a column formated file, if a label is found that
     *   does not exists as a MDLabel, it is ignored. For further
     *   file processing, such columns must be ignored and this structure
     *   allows to do that
     **/
    std::vector<unsigned int> ignoreLabels;

    /** Read, read data from an input stream and fill metadata
     * @ingroup MetaDataIO
     */
    void read(std::ifstream *infile, std::vector<MDLabel> * labelsVector);

    /** Init, do some initializations tasks, used in constructors
     * @ingroup MetaDataConstructors
     */
    void init(const std::vector<MDLabel> *labelsVector = NULL);

    /** Copy info variables from another metadata
     * @ingroup MetaDataConstructors
     */
    void copyInfo(const MetaData &md);

    /** Copy all data from another metadata
     * @ingroup MetaDataConstructors
     */
    void copyMetadata(const MetaData &md);

public:

    /// @defgroup MetaDataConstructors Constructors for MetaData objects
    /// @ingroup MetaDataClass

    /** Empty Constructor.
     * @ingroup MetaDataConstructors
     *
     * The MetaData is created with no data stored on it. You can fill in it programmatically
     * or by a later reading from a MetaData file or old Xmipp formatted type.
     * if labels vectors is passed this labels are created on metadata
     */
    MetaData();
    MetaData(const std::vector<MDLabel> *labelsVector);

    /** From File Constructor.
     * @ingroup MetaDataConstructors
     *
     * The MetaData is created and data is read from provided fileName. Optionally, a vector
     * of labels can be provided to read just those required labels
     */
    MetaData(const FileName &fileName, const std::vector<MDLabel> *labelsVector = NULL);

    /** Copy constructor
     * @ingroup MetaDataConstructors
     *
     * Created a new metadata by copying all data from an existing MetaData object.
     */
    MetaData(const MetaData &md);

    /** Assignment operator
     * @ingroup MetaDataConstructors
     *
     * Copies MetaData from an existing MetaData object.
     */
    MetaData& operator =(const MetaData &md);

    /** Destructor
     * @ingroup MetaDataConstructors
     *
     * Frees all used memory and destroys object.
     */
    ~MetaData();

    /**Clear clear all data
     * @ingroup MetaDataConstructors
     */
    void clear();



    /// @defgroup GettersAndSetters some getters and setters functions
    /// @ingroup MetaDataClass
    /// all getter should be 'const'

    /**Get column format info.
     *@ingroup GettersAndSetters
     */
    bool getColumnFormat() const;

    /** Set to false for row format (parameter files)
     *  @ingroup GettersAndSetters
     *  set to true  for column format (this is the default) (docfiles)
     *
     */
    void setColumnFormat(bool column);
    /**Get path info.
     *@ingroup GettersAndSetters
     */
    std::string getPath()   const ;
    /**Set Path
     * @ingroup GettersAndSetters
     * will appear in first line
     */
    void setPath(std::string newPath = "");
    /**Get Header Comment
     *@ingroup GettersAndSetters
     *will appear in second line
     */
    std::string getComment() const;
    /**Set Header Comment
     *@ingroup GettersAndSetters
     *will appear in second line
     */
    void setComment(const std::string newComment = "No comment");

    /**Return metadata filename
     *@ingroup GettersAndSetters
     */
    FileName getFilename();

    /**Get safetly access to active labels
     * @ingroup GettersAndSetters
     */
    std::vector<MDLabel> getActiveLabels() const;
    /**Get maximum string length of this value
    * @ingroup GettersAndSetters
    */
    int MaxStringLength( const MDLabel thisLabel) const;


    /// @defgroup DataAccess Access to MetaData data for read or write values
    /// @ingroup MetaDataClass

    /** Set the value for some label in the object that has id 'objectID'
     * @ingroup DataAccess
     */
    template<class T>
    bool setValue(const MDLabel name, const T &value, long int objectID=-1)
    {
        REPORT_ERROR(-55, "setValue(template) not yet implemented");
    }
    // Set a new pair/value for an specified object. If no objectID is given, that
    // pointed by the class iterator is used
    bool setValue(const std::string &name, const std::string &value,
                  long int objectID = -1);

    // The following 2 are for error reporting on unsupported types
    bool setValue(const std::string &name, const float &value,
                  long int objectID = -1);
    bool setValue(const std::string &name, const char &value,
                  long int objectID = -1);

    template<class T>
    bool getValue(const MDLabel name, T &value, long int objectID = -1) const
    {
        REPORT_ERROR(-55, "getValue(template) not yet implemented");
    }
    /**IsEmpty check whether the metadata is empty or not
     * @ingroup DataAccess
     */
    bool isEmpty() const;

    /**size return the number of objects contained in the metadata
     * @ingroup DataAccess
     */
    size_t size() const;

    /** Adds a new, empty object to the objects map. If objectID == -1
     * @ingroup DataAccess
     *   the new ID will be that for the last object inserted + 1, else
     *   the given objectID is used. If there is already an object whose
     *   objectID == input objectID, just removes it and creates an empty
     *   one
     **/
    long int addObject(long int objectID = -1);

    /** Import objects from another metadata
     * @ingroup DataAccess
     */
    void importObjects(const MetaData &md, const std::vector<long int> &objectsToAdd);

    void importObjects(const MetaData &md, MDQuery query);

     /**Remove the object with this id
     * @ingroup DataAccess
     * Returns true if the object was removed or false if
     * the object did not exist
     */
    bool removeObject(long int objectID);

    /** Removes the collection of objects of given vector id's
     * @ingroup DataAccess
     * NOTE: The iterator will point to the first object after any of these
     * operations
     */
    void removeObjects(const std::vector<long int> &toRemove);

    /** Removes objects with pair <label, value>
     * @ingroup DataAccess
     */
    void removeObjects(MDQuery query);

    //FIXME: organize this
    // Possible error codes for the metadata operations
    enum MDErrors
    {
        NO_OBJECTS_STORED = -1, // NOTE: Do not change this value (-1)
        NO_MORE_OBJECTS = -2,
        NO_OBJECT_FOUND = -3
    };

    ///@defgroup MetaDataIteration Some functions related with iteration over metadata objects
    ///@ingroup MetaDataClass

    /**firstObject, move the 'activeObject' to the first object in the metadata
     *@ingroup MetaDataIteration
     * */
    long int firstObject();
    long int nextObject();
    long int lastObject();
    long int goToObject(long int objectID);


    ///@defgroup MetaDataSearch Some functions for perform searches on metadata objects
    ///@ingroup MetaDataClass

    /** Find all objects with pair <label, value>
     * @ingroup MetaDataSearch
     */
    std::vector<long int> findObjects(MDQuery query);

    /**Count all objects with pairs <label, value>
     * @ingroup MetaDataSearch
     */
    int countObjects(MDQuery query);

    /** Find if the object with this id is present in the metadata
     * @ingroup MetaDataSearch
     */
    bool existsObject(long int objectID);

    /**Check if exists at least one object with pair <label, value>
     * @ingroup MetaDataSearch
     */
    bool existsObject(MDQuery query);

    /**Move active object to the first
     * object with pair <label, value in range> if exists
     * @ingroup MetaDataSearch
     */
    long int gotoFirstObject(MDQuery query);


    ///@defgroup MetaDataIO functions related to the I/O of metadata
    ///@ingroup MetaDataClass

    /** Write, write metadata to disk, using filename
     * @ingroup MetaDataIO
     */
    void write(const std::string &fileName);
    /** Read, read data from file, using filename
     * @ingroup MetaDataIO
     */
    void read(FileName infile, std::vector<MDLabel> * labelsVector = NULL);

    /// @defgroup SetOperations Set operations on MetaData's
    /// @ingroup MetaDataClass

    /** union of  metadata objects,
     *@ingroup SetOperations
     * result in calling metadata object
     * union is a reserved word so I called this method union_
     */
    void union_(const MetaData &MD, MDLabel thisLabel=MDL_OBJID);

    /** union of  metadata objects,
     * @ingroup SetOperations
     * result in calling metadata object
     * Repetion are allowed
     */
    void unionAll(const MetaData &MD);

    /** Aggregate metadata objects,
     * @ingroup SetOperations
     * result in calling metadata object
     * thisLabel label is used for aggregation, second. Valid operations are:
     *
     * MDL_AVG:  The avg function returns the average value of all  operationLabel within a group.
      The result of avg is always a floating point value as long as at there
      is at least one non-NULL input even if all inputs are integers.
       The result of avg is NULL if and only if there are no non-NULL inputs.

      MDL_COUNT: The count function returns a count of the number of times that operationLabel is in a group.

      MDL_MAX       The max aggregate function returns the maximum value of all values in the group.

      MDL_MIN       The min aggregate function returns the minimum  value of all values in the group.

     MDL_SUM The total aggregate functions return sum of all values in the group.
     If there are no non-NULL input rows then returns 0.0.

     The result of total() is always a floating point value.
     */
    void aggregate(MetaData MDIn,
                   MDLabel aggregateLabel,
                   MDLabel entryLabel,
                   MDLabel operationLabel);

    /** merge of a metadata
     * @ingroup SetOperations
     * This function reads another metadata and makes a union to this one
     */
    void merge(const FileName &fn);

    /** Aggregate modes */
    enum AggregateMode {
        KEEP_OLD,
        KEEP_NEW,
        SUM
    };

    /** intersects two metadata objects,
     *@ingroup SetOperations
     * result in "calling" metadata
     */
    void intersection(MetaData & minuend, MetaData & ,
                      MDLabel thisLabel);

    /** substract two metadata objects,
     * @ingroup SetOperations
     * result in "calling" metadata
     */
    void substraction(MetaData & minuend, MetaData & subtrahend,
                      MDLabel thisLabel);



    //---------TO ORGANIZE-------------------------
    /** Randomize this metadata, MDin is input
    */
    void randomize(MetaData &MDin){}
    /*
    * Sort this metadata, by label
    * dirty implementation using sqlite
    */
    void sort(MetaData & MDin, MDLabel sortlabel){}

    /** Split metadata into two random halves
    *
    */
    void split_in_two(MetaData &SF1, MetaData &SF2, MDLabel sortlabel=MDL_UNDEFINED){}

    /** Select only a piece of this MetaData for MPI executions
    *
    */
    void mpi_select_part(int rank, int size, int &num_img_tot){}

    /** Fill metadata with N entries from MD starting at start
    *
    */
    void fillWithNextNObjects (MetaData &MD, long int start, long int numberObjects){}



};

///@defgroup MetaDataQuery represent queries to a metadata
//@ingroup DataLibrary

/** MDQuery this is the base class for queries, its abstract
 *@ingroup MetaDataQuery
 */
class MDQuery
{
public:

    /**This now is specific to the SQL implementation
     * and its requiered to all MDQuery subclasses
     * equal to 0 means that is ABSTRACT in this class
     * and only accesible for MetaData
     */
    std::string queryString;
};

/**MDValueEqual this will test if a label have a value
 *@ingroup MetaDataQuery
 */
class MDValueEqual: public MDQuery
{
public:
    template <class T>
    MDValueEqual(MDLabel label, const T &value)
    {
        //TODO: create the query string
    }
};//class MDValueEqual

/**MDValueEqual this will test if a label have a value
 *@ingroup MetaDataQuery
 */
class MDValueRange: public MDQuery
{
public:
    template <class T>
    MDValueRange(MDLabel label, const T &valueMin, const T &valueMax)
    {
        //TODO: create the query string
    }
};//class MDValueRange


/** For all objects.
 @code
 FOR_ALL_OBJECTS_IN_METADATA(metadata) {
 double rot; DF.getValue( MDL_ANGLEROT, rot);
 }
 @endcode
 */
//Write for partial metadatas- read use NULL
//better sort
//error in metadata_split when there is only one comment
//COMMENT

#define FOR_ALL_OBJECTS_IN_METADATA(kkkk_metadata) \
        for(long int kkkk = (kkkk_metadata).firstObject(); \
             kkkk != MetaData::NO_MORE_OBJECTS && kkkk!= MetaData::NO_OBJECTS_STORED; \
             kkkk=(kkkk_metadata).nextObject())
#endif
