/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*/
/*
 * Author: Suman Raj Bista
 * Initial version @Inria Rennes Breatgne Atlantique
 * Modified for navigation of the Pepper Robot @ACRV QUT
 */



#ifndef LINEMATCH_H
#define LINEMATCH_H


#include "LineDescriptor.hh"
#include "PairwiseLineMatching.hh"

class linematch
{
    PairwiseLineMatching *lm;
    LineDescriptor *ld;
    //std::vector<int> tmp;
public:

    /**
     * @brief linematch
     */

    linematch();


   // int getlinedesc(BIAS::Image<unsigned char> &leftImage,ScaleLines &linesInLeft);
   // int getlinedesc(BIAS::Image<unsigned char> &leftImage,ScaleLines &linesInLeft,std::string linefile);

    /**
     * @brief readlinedesc read line descriptors from file
     * @param keyln file containing line segemts and descriptors
     * @param linesInRight line segemnts and descriptors in ScaleLine format
     */

    void readlinedesc(std::string keyln, ScaleLines &linesInRight);


    /**
     * @brief matchlines matches lines based on LBD descriptors
     * @param linesInLeft
     * @param linesInRight
     * @param matchResult index of matched pairs
     */

    void matchlines(ScaleLines &linesInLeft,ScaleLines &linesInRight,std::vector<unsigned int> &matchResult);


    /**
     * @brief matchlinesbinary  matches lines based on binary descriptors
     * @param linesInLeft
     * @param linesInRight
     * @param matchResult
     */

    void matchlinesbinary(ScaleLines &linesInLeft,ScaleLines &linesInRight,std::vector<unsigned int> &matchResult);


    /**
     * @brief findCommonIndex find common index => for 3 view line matching between im1, im2 and im3
     * @param v1 match pair between im1 im2
     * @param v2 match pair between im1 and im3
     * @param op matched index
     */

    void findCommonIndex(std::vector<unsigned int> v1,std::vector<unsigned int> v2, std::vector< std::vector< int> >&op);




    void findCommonIndex2(std::vector<unsigned int> v1,std::vector<unsigned int> v2, std::vector< std::vector< int> >&op);


    /**
     * @brief getlinedesc calulae line descriptos
     * @param leftImage image
     * @param linesInLeft line descriptors
     * @return
     */

    int getlinedesc(cv::Mat &leftImage,ScaleLines &linesInLeft);



private:


};

#endif // LINEMATCH_H
