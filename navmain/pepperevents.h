/* header files to read the Pepper's internal events related to move and collision
 *
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
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
 */



#ifndef PEPPEREVENTS_H
#define PEPPEREVENTS_H

#include <qi/anyobject.hpp>

/**
 * @brief The pepperServices class
 */
class pepperServices{
public:
    bool eventraised;

    /**
     * @brief pepperServices
     * @param almemory AL::Memory pointer
     */
    pepperServices(qi::AnyObject &almemory){
          almemory.call<void>("subscribeToEvent","ALMotion/MoveFailed","pepperServices","moveCallback");
       // almemory.call<void>("subscribeToEvent","ALMotion/Safety/ChainVelocityClipped","pepperServices","armsCallback");
       // almemory.call<void>("subscribeToMicroEvent","ALMotion/MoveFailed","pepperServices","moveCallback","moveCallback");
          almemory.call<void>("subscribeToMicroEvent","ALMotion/Safety/ChainVelocityClipped","pepperServices","armsCallback","armsCallback");
          eventraised = true;
    }

    /**
     * @brief moveCallback
     */

    void moveCallback(const std::string &key, const qi::AnyValue &value, const  qi::AnyValue &message){

        qi ::AnyValue fd = value;
        qi ::AnyValue ms = message;
        std::cout<<"\n ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"<<std::endl;
        std::cout<<"evet raised" <<"\t \t"<<fd.size()<<std::endl;
        std::cout<<key<<std::endl;
        std::cout<<"mf = "<<fd[0].to<std::string>()/*<<"\t"<<fd[1].to<int>()<<"\t"<<"( "<<wc[0]<<" , "<<wc[1]<<" , "<<wc[2]<<" ) "*/<<std::endl;
        std::cout<<"ms ="<<ms.as<std::string>()<<std::endl;
        std::cout<<"^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"<<std::endl;
         //robo->setBaseVelocity(0.0,0.0,0.0);
        eventraised = true;

    }

    /**
     * @brief armsCallback
     */

    void armsCallback(const std::string &key, const qi::AnyValue &value, const  qi::AnyValue &message){

        qi ::AnyValue fd = value;
        eventraised = true;
        std::cout<<"\n ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"<<std::endl;
        qi ::AnyValue ms =    message;
        std::cout<<"evet raised" <<std::endl;
        std::cout<<key<<std::endl;

        if(fd.size()>0){
            qi::AnyValue rv = fd[0].to<qi::AnyValue>();
            std::cout<<rv[0].to<std::string>()<<"\t";
             //std::vector<double> wc = rv[1].to<std::vector<double>>();
             std::cout/*<<"\t"<<"( "<<wc[0]<<" , "<<wc[1]<<" , "<<wc[2]<<" ) "*/<<std::endl;
        }

        else{
            std::cout<<"Both"<<std::endl;
        }

        std::cout<<"ms ="<<ms.as<std::string>()<<std::endl;
        std::cout<<"^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"<<std::endl;
    }


 };


/**
 *  Register Callback Services
 */

QI_REGISTER_OBJECT(pepperServices, moveCallback,armsCallback);

#endif // PEPPEREVENTS_H
