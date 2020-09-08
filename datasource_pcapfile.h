/*
    This file is part of Kismet

    Kismet is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    Kismet is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Kismet; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef __DATASOURCE_PCAPFILE_H__
#define __DATASOURCE_PCAPFILE_H__

#include "config.h"

#include "kis_datasource.h"

class kis_datasource_pcapfile;
typedef std::shared_ptr<kis_datasource_pcapfile> shared_datasource_pcapfile;

class kis_datasource_pcapfile : public kis_datasource {
public:
    kis_datasource_pcapfile(shared_datasource_builder in_builder) :
        kis_datasource(in_builder) {

        // Set the capture binary
        set_int_source_ipc_binary("kismet_cap_pcapfile");
    }

    virtual ~kis_datasource_pcapfile() { };

    // Almost all of the logic is implemented in the capture binary and derived
    // from our prototype; all the list, probe, etc functions proxy to our binary
    // and we communicate using only standard Kismet functions so we don't need
    // to do anything else
   
    // Override defaults for pcapfile - we don't want to reload a pcapfile once
    // it finishes unless we're explicitly told to loop it
    virtual std::string override_default_option(std::string in_opt) override {
        if (in_opt == "retry")
            return "false";

        return "";
    }
    
};


class datasource_pcapfile_builder : public kis_datasource_builder {
public:
    datasource_pcapfile_builder() :
        kis_datasource_builder() {

        register_fields();
        reserve_fields(NULL);
        initialize();
    }

    datasource_pcapfile_builder(int in_id) :
        kis_datasource_builder(in_id) {

        register_fields();
        reserve_fields(NULL);
        initialize();
    }

    datasource_pcapfile_builder(int in_id, std::shared_ptr<tracker_element_map> e) :
        kis_datasource_builder(in_id, e) {

        register_fields();
        reserve_fields(NULL);
        initialize();
    }

    virtual ~datasource_pcapfile_builder() { }

    virtual shared_datasource build_datasource(shared_datasource_builder in_sh_this) override {
        return shared_datasource_pcapfile(new kis_datasource_pcapfile(in_sh_this));
    }

    virtual void initialize() override {
        // Set up our basic parameters for the pcapfile driver
        
        set_source_type("pcapfile");
        set_source_description("Pre-recorded pcap or pcapng file");

        // We can probe an 'interface' and see if it's a local pcap file
        set_probe_capable(true);

        // We can't list interfaces - trying to list pcap files doesn't make much sense
        set_list_capable(false);

        // We're capable of opening a source
        set_local_capable(true);

        // We can do remote pcapfile for testing
        set_remote_capable(true);

        // We don't accept packets over HTTP passively, though I guess we could
        set_passive_capable(false);

        // Can't change channel on a pcapfile
        set_tune_capable(false);
    }

};

#endif

