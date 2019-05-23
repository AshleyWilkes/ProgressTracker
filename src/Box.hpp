#pragma once
#include "Request.hpp"

#include "boost/range/iterator_range_core.hpp"
#include "boost/serialization/vector.hpp"

#include <vector>

namespace ashley::progress_tracker {
  template<typename PS>
  class Box {
    public:
      using PS_Cont = std::vector<PS>;
      using PS_Iter = typename PS_Cont::const_iterator;
      using PS_Range = boost::iterator_range<PS_Iter>;

      //toto nema byt tady, takhle to zavisi na template parametrech
      enum class Status{ Empty, HalfFull, Full };

      //bezparametricky konstruktor nema vubec existovat, je tu kvuli
      //serializaci, ale to je spatne; serializace asi nema pouzivat "rekurzivni"
      //volani serializace "vnitrnich" trid.
      Box() : Box( -1 ) {}
      Box( std::size_t totalCapacity );

      std::size_t getTotalCapacity() const { return totalCapacity_; }
      std::size_t getAvailableCapacity() const { return totalCapacity_ - usedCapacity_; }
      std::size_t getUsedCapacity() const { return usedCapacity_; }

      Status getStatus() const;

      PS_Range getProgressSteps() const { return progressSteps_; }

      void processRequest( Request<PS>& request );

      template<typename Archive>
      void serialize( Archive& ar, const unsigned int version );
    private:
      std::size_t totalCapacity_;
      std::size_t usedCapacity_;

      PS_Cont progressSteps_;
  };

  template<typename PS>
  Box<PS>::Box( std::size_t totalCapacity ) : totalCapacity_{ totalCapacity }, usedCapacity_{ 0 }, progressSteps_{} {
    if ( totalCapacity < 1 ) {
      throw std::domain_error{ "Invalid total capacity: " + std::to_string( totalCapacity ) };
    }
  }

  template<typename PS>
  typename Box<PS>::Status
  Box<PS>::getStatus() const {
    if ( usedCapacity_ == 0 ) {
      return Status::Empty;
    } else if ( getAvailableCapacity() == 0 ) {
      return Status::Full;
    } else {
      return Status::HalfFull;
    }
  }

  template<typename PS>
  void
  Box<PS>::processRequest( Request<PS>& request ) {
    //zvolit, jakou kapacitu requestu nabidnu
    //toto je asi kandidat na virtualni metodu misto natvrdo zapsaneho
    //std::min; napr bych mohl mit box, ktery neumoznuje obsadit najednou
    //vetsi nez urcitou kapacitu nebo kdovico vsechno
    std::size_t actualCapacity = std::min( getAvailableCapacity(), request.getAvailableCapacity() );
    //rict si requestu o progressStep
    auto progressStep = request.useCapacity( actualCapacity );
    //upravit podle kapacity progressStepu svoje kapacity
    usedCapacity_ += progressStep->getCapacity();
    //pridat progressStep do kontejneru
    progressSteps_.push_back( *( progressStep.get() ) );
  }

  template<typename PS>
  template<typename Archive>
  void
  Box<PS>::serialize( Archive& ar, const unsigned int version ) {
    ar & totalCapacity_;
    ar & usedCapacity_;
    //tohle funguje jen pro ProgressStepy, ktere maji konstruktor, ktery jde zavolat
    //bez parametru!! Coz ale nechci, prozkoumat serialization.
    //tady by melo pekne zafungovat, kdyz by progressSteps_ nebyl kontejner primo
    //<PS>, ale unique_ptr<PS>
    ar & progressSteps_;
  }

  template<typename PS>
  bool operator==( const Box<PS>& lhs, const Box<PS>& rhs ) {
    return ( lhs.getTotalCapacity() == rhs.getTotalCapacity() )
      && ( lhs.getUsedCapacity() == rhs.getUsedCapacity() )
      && ( lhs.getProgressSteps() == rhs.getProgressSteps() );
  }

  template<typename PS>
  bool operator!=( const Box<PS>& lhs, const Box<PS>& rhs ) {
    return ! ( lhs == rhs );
  }
}
