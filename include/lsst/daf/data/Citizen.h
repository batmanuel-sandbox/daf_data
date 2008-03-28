// -*- lsst-c++ -*-
#if !defined(LSST_SHARED_POINTERS)      //! multiple inclusion guard macro
#define LSST_SHARED_POINTERS 1

#include <vector>
#include <map>

#include "lsst/mwi/utils/Utils.h"

namespace lsst {
namespace mwi {
namespace data {
/*! \brief Citizen is a class that should be among all LSST
 * classes base classes, and handles basic memory management
 *
 * Instances of subclasses of Citizen will automatically be
 * given a unique id.
 *
 * You can ask for infomation about the currently allocated
 * Citizens using the census functions, request that
 * a function of your choice be called when a specific
 * block ID is allocated or deleted, and check whether any
 * of the data blocks are known to be corrupted
 */
    class Citizen {
    public:
        //! Type of the block's ID
        typedef unsigned long memId;
        //! A function used to register a callback
        typedef memId (*memCallback)(const Citizen *ptr);

        Citizen(const std::type_info &);
        Citizen(Citizen const &);
        ~Citizen();

        Citizen & operator=(Citizen const &) { return *this; }
        //
        std::string repr() const;
        void markPersistent(void);

        static int census(int, memId startingMemId = 0);
        static void census(std::ostream &stream, memId startingMemId = 0);
        static const std::vector<const Citizen *> *census();

        static bool checkCorruption();
        
        memId getId() const;
        
        static memId getNextMemId();

        static memId setNewCallbackId(memId id);
        static memId setDeleteCallbackId(memId id);
        static memCallback setNewCallback(memCallback func);
        static memCallback setDeleteCallback(memCallback func);
        static memCallback setCorruptionCallback(memCallback func);
        //
        enum { magicSentinel = 0xdeadbeef }; //!< a magic known bit pattern
        static int init();
    private:
        typedef std::map<memId, const lsst::mwi::data::Citizen *> table;

        int _sentinel;                  // Initialised to _magicSentinel to detect overwritten memory
        memId _CitizenId;               // unique identifier for this pointer
        const char *_typeName;          // typeid()->name
        //
        // Book-keeping for _CitizenId
        //
        static memId& _nextMemId(void);
        static table& activeCitizens();
        static table& permanentCitizens();
        //
        // Callbacks
        //
        static memId _newId;       // call _newCallback when _newID is allocated
        static memId _deleteId;    // call _deleteCallback when _deleteID is deleted

        static memCallback _newCallback;
        static memCallback _deleteCallback;        
        static memCallback _corruptionCallback;        
        //
        bool _checkCorruption() const;
    };

} // namespace data
} // namespace mwi
} // namespace lsst
#endif
