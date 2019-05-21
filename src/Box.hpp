#pragma once
#include "boost/range/iterator_range_core.hpp"
#include "boost/serialization/vector.hpp"
#include "Request.hpp"
#include <vector>

namespace ashley::progress_tracker {
  template<typename PS, template<typename> typename Cont = std::vector>
  class Box {
    public:
      using PS_Cont = Cont<PS>;
      using PS_Iter = typename PS_Cont::const_iterator;
      using PS_Range = boost::iterator_range<PS_Iter>;

      //toto nema byt tady, takhle to zavisi na template parametrech
      enum class Status{ Empty, HalfFull, Full };

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

  template<typename PS, template<typename> typename Cont>
  Box<PS, Cont>::Box( std::size_t totalCapacity ) : totalCapacity_{ totalCapacity }, usedCapacity_{ 0 }, progressSteps_{} {
    if ( totalCapacity < 1 ) {
      throw std::domain_error{ "Invalid total capacity: " + std::to_string( totalCapacity ) };
    }
  }

  template<typename PS, template<typename> typename Cont>
  typename Box<PS, Cont>::Status
  Box<PS, Cont>::getStatus() const {
    if ( usedCapacity_ == 0 ) {
      return Status::Empty;
    } else if ( getAvailableCapacity() == 0 ) {
      return Status::Full;
    } else {
      return Status::HalfFull;
    }
  }

  template<typename PS, template<typename> typename Cont>
  void
  Box<PS, Cont>::processRequest( Request<PS>& request ) {
    //zvolit, jakou kapacitu requestu nabidnu
    std::size_t actualCapacity = std::min( getAvailableCapacity(), request.getAvailableCapacity() );
    //rict si requestu o progressStep
    auto progressStep = request.useCapacity( actualCapacity );
    //upravit podle kapacity progressStepu svoje kapacity
    usedCapacity_ += progressStep->getCapacity();
    //pridat progressStep do kontejneru
    progressSteps_.push_back( *( progressStep.get() ) );
  }

  template<typename PS, template<typename> typename Cont>
  template<typename Archive>
  void
  Box<PS, Cont>::serialize( Archive& ar, const unsigned int version ) {
    ar & totalCapacity_;
    ar & usedCapacity_;
    //tohle funguje jen pro ProgressStepy, ktere maji konstruktor, ktery jde zavolat
    //bez parametru!! Coz ale nechci, prozkoumat serialization.
    ar & progressSteps_;
  }

  template<typename PS, template<typename> typename Cont>
  bool operator==( const Box<PS, Cont>& lhs, const Box<PS, Cont>& rhs ) {
    return ( lhs.getTotalCapacity() == rhs.getTotalCapacity() )
      && ( lhs.getUsedCapacity() == rhs.getUsedCapacity() )
      && ( lhs.getProgressSteps() == rhs.getProgressSteps() );
  }

  template<typename PS, template<typename> typename Cont>
  bool operator!=( const Box<PS, Cont>& lhs, const Box<PS, Cont>& rhs ) {
    return ! ( lhs == rhs );
  }
}
