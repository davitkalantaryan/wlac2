/**
 * \file eq_client.h
 * \brief Declaration of the EqCall class.
 * \author Kay Rehlich, Olaf Hensler, Gerhard Schlesselmann, Arthur Aghababyan
 * \date October 24, 1995
 *
 * \copyright Copyright 1995-2018 Deutsches Elektronen-Synchrotron (DESY), Hamburg
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 2.1 of the license, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

/**
 * \mainpage
 *
 * \section intro Introduction
 *
 * The DOOCS client library provides the interface to RPC based communication.
 * The main class for initiating communication is EqCall, data is passed as EqData
 * objects, and control system addresses are specified via the EqAdr class.
 *
 * A transfer always needs an address defined by an object of the EqAdr
 * class and returns a data object of EqData class. Some calls also need
 * a data object to be send to the device. The current implementation has
 * 3 communication methods - a get, a set and a names calls. The get call
 * is used read device data, the set call sets device data and the names
 * call returns a list with the available names in the communication network.
 * All calls return a pointer to an EqData structure. This structure is
 * freed on the next call of get/set/names. To keep the data it is necessary
 * to copy the result or to create a further EqCall instance. "disconnect"
 * disconnects all servers from this application. Three additional calls are
 * used to operate with monitors. A monitor buffers the data from one property
 * (channel) in a local EqData structure. The data is updated by an update call.
 * A first call of get_monitor reads the data from the device and creates
 * a monitor. Further calls get the data from the buffer without doing
 * a network call. This interface is prepared to use EPICS monitors also.
 * The user program needs not to be changed when this feature is added later.
 *
 * \subsection example_1 Example: Read data from an address
 *
 * \code
 * #include <eq_client.h>
 *
 * EqCall eq;
 * EqData data_in, data_out;
 * EqAdr  ea("TTF.VAC/MASS_SPECTR/VERT/RANGE");
 *
 * int err = eq.get(&ea, &data_in, &data_out);
 * if (err)
 *     std::cout << data_out.get_string() << "\n";
 * else
 *     std::cout << data_out.get_float() << "\n";
 * \endcode
 *
 * The RANGE information from a mass spectrometer at position "VERT" is read and
 * the resulting data is returned in data_out. If an error occurred, the error code
 * is returned both as the return value of the call and in data_out:
 * \code
 * int err_code = data_out.error();
 * \endcode
 *
 * \subsection example_2 Example: Write data to an address
 *
 * \code
 * ea.adr("TTF.VAC/MASS_SPECTR/VERT/RANGE");
 * data_in.set(3);
 * err = eq.set(&ea, &data_in, &data_out);
 * \endcode
 */

#ifndef EQ_CLIENT_H
#define EQ_CLIENT_H

#include "eq_adr.h"
#include "eq_data.h"
#include "eq_errors.h"
#include "eq_sapi.h"

// the commands for setoption () and getoption ()

enum EqOption  {

        EQ_HOSTNAME = 1,
        EQ_PROTOCOL,
        EQ_PROTOCOLID,
        EQ_RETRY,
        EQ_ONLINE,
        EQ_TCP,
        EQ_UDP,
        EQ_AUTHTABLE,
        EQ_AUTHMASK,
        EQ_SVRNAME,
        EQ_SVRMASK,
        EQ_SVRSTATUS,
        EQ_SVROPTION,
        EQ_FILE_CHANNEL,
        EQ_TIMEOUT,

        EQ_UPDATEMASK,

        EQ_SHM,
        EQ_SHM_CREATE,
        EQ_SHM_DELETE,

        EQ_UPDATEMODE,
        EQ_LIFETIME,

        EQ_CALLTIME,
        EQ_YIELD,

        EQ_LIBNO,
        EQ_DEBUG,

        EQ_TINEVERS,
        EQ_DESTROY,

        EQ_TINECYCLER,
        EQ_TINETMO,
        EQ_TINECONS
};


// the bit fields in the update_mask of get_monitor ()

#define  UPDATE_NORMAL            (1)
#define  UPDATE_FAST              (1 << 1)
#define  UPDATE_SLOW              (1 << 2)
#define  UPDATE_ONCE              (1 << 3)
#define  UPDATE_BY_SYSMASK        (1 << 4)
#define  UPDATE_SUPPRESS_REPLICAS (1 << 5)
#define  UPDATE_FIRST_TIME        (1 << 16)

#define  UPDATE_ON_CHANGE         (1 << 17)
#define  UPDATE_CONNECTED         (1 << 18)


// callback function prototype for get_monitor ()

typedef void (*EQ_CB) (EqData *, void *);

#define  EQ_IGNORE                ((EQ_CB) -1)
#define  EQ_RESET                 ((EQ_CB) 0)



class   EqCall {

private:
        static void    *pdap;

        void           *ensp;
        void           *shmp;
        void           *resp;

        void           *semp;
        EqData         data;

public:
                   EqCall         (void);
                   ~EqCall        (void);

        // new MT-safe API

        /**
         * Query control system names from the network.
         *
         * The names call is used to query names from the network. A wild card character
         * ("*") in one of the 4 components of the address in the names call selects the
         * list which will be returned. The data type of the returned data is always
         * DATA_A_USTR. Currently the "*" is the only supported wildcard character.
         * A "*" in the facility, device, location or property field gives a list of the
         * according facilities, devices, locations or properties. The list of names are
         * in the ASCII part of the DATA_A_USTR structure. The list of properties contains
         * the data type in the integer part of the DATA_A_USTR structure also. A
         * description of the property is appended to the property name string and
         * separated by a blank character.
         *
         * \param adr   Pointer to an EqAdr control system address. This address must
         *              contain a wildcard.
         * \param data  Pointer to an EqData object that is filled with the results of the
         *              call.
         * \returns 0 if everything went well, or an error code otherwise.
         */
        int        names          (EqAdr *adr, EqData *data);

        /**
         * Perform a get-call over the network.
         *
         * The get call sends a data and an address object to the device and reads back
         * a data object. The data sent to the device is only usefull for some properties
         * to specify a time range or to select a stored spectrum for instance.
         *
         * \param adr      Pointer to an EqAdr control system address.
         * \param data_in  Pointer to an EqData object with data that will be sent to the
         *                 remote server.
         * \param data_out Pointer to an EqData object that will receive any output data
         *                 from the remote server or an error message.
         * \returns 0 if everything went well, or an error code otherwise.
         */
        int        get            (EqAdr *adr, EqData *data_in, EqData *data_out);

        /**
         * Perform a set-call over the network.
         *
         * The set call sends a data and an address object to a device and gets a result
         * data object returned. The data returned data contains the error messages of
         * the call or any other data that the remote server chose to return.
         *
         * \param adr      Pointer to an EqAdr control system address.
         * \param data_in  Pointer to an EqData object with data that will be sent to the
         *                 remote server.
         * \param data_out Pointer to an EqData object that will receive any output data
         *                 from the remote server or an error message.
         * \returns 0 if everything went well, or an error code otherwise.
         */
        int        set            (EqAdr *adr, EqData *data_in, EqData *data_out);

        /**
         * Retrieve communication parameters.
         *
         * The get_option() call can be used to access most of the internal settings of
         * the DOOCS client library.
         *
         * \verbatim
           EqOption  read/write Function

           EQ_HOSTNAME     R    hostname of the server
           EQ_PROTOCOL     R    protocol name of communication
           EQ_PROTOCOLID   R    protocol as integer id
           EQ_RETRY        W    resets the retry counter and reconnects
           EQ_ONLINE       R    device is online flag from ENS
           EQ_TCP          RW   use TCP protocol (DOOCS only) *)
           EQ_UDP          RW   use UDP protocol (DOOCS only) *)
           EQ_AUTHTABLE    R    shows who has access permissions
           EQ_AUTHMASK     R    shows authentication for this client
           EQ_SVRNAME      R    name of the server
           EQ_SVRMASK      R    server mask: part of name which points to the server *)
           EQ_SVRSTATUS    R    connection status
           EQ_SVROPTION    R    available server options
           EQ_FILE_CHANNEL R    file or channel name argument from ENS
           EQ_TIMEOUT      RW   timeout parameter in calls
           EQ_UPDATEMASK   RW   priority of updates (UPDATE_NORMAL, UPDATE_FAST, UPDATE_SLOW)
           EQ_SHM          RW   shows that the shm creator-process exists (R).
                                On (W) deletes its name when creator does not exist
           EQ_SHM_CREATE   W    creates the shared memory of the type specified in EqData
                                and the name specified in EqAdr
           EQ_SHM_DELETE   W    deletes the shared memory specified in EqAdr
           EQ_UPDATEMODE   RW   synchronous/asynchronous mode for get_monitor ()
                                (IIII type in EqData):
                                   i1_data: update interval timer (sec)  (for FAST);
                                   i2_data: update interval timer (usec) (for FAST);
                                   i3_data: decrement count (for NORMAL);
                                   i4_data: decrement count (for SLOW);
           EQ_LIFETIME     RW   defines the duration of the existence of the monitor
                                created by get_monitor ();
           EQ_YIELD        RW   enables/disables 100 ms pause between monitor updates
           EQ_LIBNO        RO   program rpc number
           EQ_DEBUG        RW   tine debugging
           EQ_TINEVERS     RO   shows tine layer version
           EQ_DESTROY      WO   destroys a connection to the server

           *) not yet implemented
         \endverbatim
         */
        int        get_option     (EqAdr *, EqData *, EqData *, EqOption);

        /**
         * Set communication parameters.
         *
         * The set_option() call can be used to manipulate various settings of
         * the DOOCS client library. For a list of available options see get_option().
         */
        int        set_option     (EqAdr *, EqData *, EqData *, EqOption);

        /**
         * Create an asynchronous monitor on a control system address.
         *
         * Reads back the data from a buffer (async call). It creates a monitor instance.
         * Further calls to the same address read from the monitor instance only. All
         * monitors are updated by the by the update call. The options argument specifies
         * an update mask which must match one or more mask bits in the update call. The
         * following bits are defined:
         *
         * UPDATE_NORMAL, UPDATE_FAST, UPDATE_SLOW, UPDATE_ONCE, UPDATE_BY_SYSMASK,
         * UPDATE_SUPPRESS_REPLICAS, UPDATE_FIRST_TIME, UPDATE_ON_CHANGE, UPDATE_CONNECTED
         *
         * The rate argument is used to specify the maximum refresh rate of the device
         * server in seconds (for TINE calls only, float parameter).
         *
         * \param adr      Pointer to an EqAdr control system address that should be
         *                 monitored.
         * \param data_in  Pointer to an EqData object with data that will be sent to the
         *                 remote server.
         * \param data_out Pointer to an EqData object that will receive any output data
         *                 from the remote server or an error message.
         * \param options  An integer bitfield that encodes various options for the
         *                 monitor through bitwise OR of some constants, e.g.
         *                 UPDATE_ON_CHANGE | UPDATE_FAST | UPDATE_SUPPRESS_REPLICAS
         * \returns 0 on success or -1 on error
         *
         * <h3>Example + Remarks</h3>
         *
         * \code
         * EqCall ec;
         * EqAdr  addr(“XFEL.MAGNETS/MAGNET.ML/BK.24.I1/CURRENT.SP”);
         * EqData data_in, data_out;
         *
         * const int mask = UPDATE_ON_CHANGE | UPDATE_FAST | UPDATE_SUPPRESS_REPLICAS;
         *
         * int err = ec.get_monitor(&addr, &data_in, &data_out, mask); // (1)
         * if (err)
         *     handle_error();
         *
         * wait_some_time();
         *
         * err = ec.get_monitor(&addr, &data_in, &data_out, mask); // (2)
         * \endcode
         *
         * Here, the first call (1) to get_monitor() creates a new monitor if it does not
         * exist yet. The second one promptly returns data, provided that wait_some_time()
         * took longer than the monitor update time.
         *
         * The monitor mechanism is done in a way that two internal threads (one dedicated
         * to fast and one to normal/slow updates) start executing calls only when the
         * update time has run out. Until that all get_monitor() calls return the error
         * code no_data. In the example, this is what happens for call (1).
         *
         * Usually monitors remain active as long as they are accessed and the access time
         * between calls is shorter than the monitor lifetime which can be get/set by
         * get_option()/set_option() with the EQ_LIFETIME parameter. Should the next
         * access happen after the lifetime has run out, the library will close the
         * monitor and open it again on the next call.
         *
         * So in order to get rid of a monitor, it is possible to call clear_monitor()
         * or to simply stop accessing it and the library will deactivate it some time
         * later (EQ_LIFETIME). It should be noted that the monitor is not deleted but
         * rather deactivated.
         */
        int        get_monitor    (EqAdr *adr, EqData *data_in, EqData *data_out,
                                   int options, float = 0.0, EQ_CB = EQ_IGNORE,
                                   void * = NULL);

        /**
         * Remove a monitor from a control system address.
         * This call clears the monitor from the list of updated entries.
         */
        int        clear_monitor  (EqAdr *);

        /**
         * Update all monitors that have the corresponding mask bit(s) set.
         * Convention for masks are: 1 = normal, 2 = fast, 4 = slow updates.
         */
        int        update         (int);

        int        servers        (void);
        int        monitors       (void);
        int        monitors_count (void) const;
        int        disconnect     (void);

        // old MT-unsafe API

        EqData     *names         (EqAdr *);
        EqData     *get           (EqAdr *, EqData *);
        EqData     *set           (EqAdr *, EqData *);
        EqData     *get_option    (EqAdr *, EqOption);
        EqData     *set_option    (EqAdr *, EqOption, EqData * = 0);
        EqData     *get_monitor   (EqAdr *, EqData *, int, float = 0.0);
};

#endif
