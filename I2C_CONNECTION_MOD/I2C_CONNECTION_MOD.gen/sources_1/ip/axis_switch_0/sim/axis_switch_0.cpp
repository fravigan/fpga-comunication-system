// (c) Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// (c) Copyright 2022-2024 Advanced Micro Devices, Inc. All rights reserved.
// 
// This file contains confidential and proprietary information
// of AMD and is protected under U.S. and international copyright
// and other intellectual property laws.
// 
// DISCLAIMER
// This disclaimer is not a license and does not grant any
// rights to the materials distributed herewith. Except as
// otherwise provided in a valid license issued to you by
// AMD, and to the maximum extent permitted by applicable
// law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND
// WITH ALL FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES
// AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, INCLUDING
// BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-
// INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE; and
// (2) AMD shall not be liable (whether in contract or tort,
// including negligence, or under any other theory of
// liability) for any loss or damage of any kind or nature
// related to, arising under or in connection with these
// materials, including for any direct, or any indirect,
// special, incidental, or consequential loss or damage
// (including loss of data, profits, goodwill, or any type of
// loss or damage suffered as a result of any action brought
// by a third party) even if such damage or loss was
// reasonably foreseeable or AMD had been advised of the
// possibility of the same.
// 
// CRITICAL APPLICATIONS
// AMD products are not designed or intended to be fail-
// safe, or for use in any application requiring fail-safe
// performance, such as life-support or safety devices or
// systems, Class III medical devices, nuclear facilities,
// applications related to the deployment of airbags, or any
// other applications that could lead to death, personal
// injury, or severe property or environmental damage
// (individually and collectively, "Critical
// Applications"). Customer assumes the sole risk and
// liability of any use of AMD products in Critical
// Applications, subject only to applicable laws and
// regulations governing limitations on product liability.
// 
// THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS
// PART OF THIS FILE AT ALL TIMES.
// 
// DO NOT MODIFY THIS FILE.


#include "axis_switch_0_sc.h"

#include "axis_switch_0.h"

#include "axis_switch_0_core.h"

#include <map>
#include <string>





#ifdef XILINX_SIMULATOR
axis_switch_0::axis_switch_0(const sc_core::sc_module_name& nm) : axis_switch_0_sc(nm), aclk("aclk"), aresetn("aresetn"), s_axis_tvalid("s_axis_tvalid"), s_axis_tready("s_axis_tready"), s_axis_tdata("s_axis_tdata"), s_axis_tlast("s_axis_tlast"), m_axis_tvalid("m_axis_tvalid"), m_axis_tready("m_axis_tready"), m_axis_tdata("m_axis_tdata"), m_axis_tlast("m_axis_tlast"), s_req_suppress("s_req_suppress"), s_decode_err("s_decode_err")
{

  // initialize pins
  mp_impl->aclk(aclk);
  mp_impl->aresetn(aresetn);
  mp_impl->s_req_suppress(s_req_suppress);
  mp_impl->s_decode_err(s_decode_err);

  // initialize transactors
  mp_S00_AXIS_transactor = NULL;
  mp_s_axis_tvalid_converter_0 = NULL;
  mp_s_axis_tready_converter_0 = NULL;
  mp_s_axis_tdata_converter_0 = NULL;
  mp_s_axis_tlast_converter_0 = NULL;
  mp_M00_AXIS_transactor = NULL;
  mp_m_axis_tvalid_converter = NULL;
  mp_m_axis_tready_converter = NULL;
  mp_m_axis_tlast_converter = NULL;
  mp_S01_AXIS_transactor = NULL;
  mp_s_axis_tvalid_converter_1 = NULL;
  mp_s_axis_tready_converter_1 = NULL;
  mp_s_axis_tdata_converter_1 = NULL;
  mp_s_axis_tlast_converter_1 = NULL;

  // initialize junctures
  mp_s_axis_concat_tready = NULL;
  mp_s_axis_split_tdata = NULL;
  mp_s_axis_split_tlast = NULL;
  mp_s_axis_split_tvalid = NULL;
  mp_s_axis_split_tvalid = new xsc::xsc_split<2, 2>("s_axis_split_tvalid");
  mp_s_axis_split_tvalid->in_port(s_axis_tvalid);
  mp_s_axis_split_tvalid->out_port[0](s_axis_split_tvalid_out_0);
    mp_s_axis_split_tvalid->add_mask(0,1,0);
  mp_s_axis_concat_tready = new xsc::xsc_concatenator<2, 2>("s_axis_concat_tready");
  mp_s_axis_concat_tready->in_port[0](s_axis_concat_tready_out_0);
  mp_s_axis_concat_tready->out_port(s_axis_tready);
    mp_s_axis_concat_tready->offset_port(0, 0);
  mp_s_axis_split_tdata = new xsc::xsc_split<16, 2>("s_axis_split_tdata");
  mp_s_axis_split_tdata->in_port(s_axis_tdata);
  mp_s_axis_split_tdata->out_port[0](s_axis_split_tdata_out_0);
    mp_s_axis_split_tdata->add_mask(0,8,0);
  mp_s_axis_split_tlast = new xsc::xsc_split<2, 2>("s_axis_split_tlast");
  mp_s_axis_split_tlast->in_port(s_axis_tlast);
  mp_s_axis_split_tlast->out_port[0](s_axis_split_tlast_out_0);
    mp_s_axis_split_tlast->add_mask(0,1,0);
  
  mp_s_axis_split_tvalid->out_port[1](s_axis_split_tvalid_out_1);
    mp_s_axis_split_tvalid->add_mask(1,2,1);
  mp_s_axis_concat_tready->in_port[1](s_axis_concat_tready_out_1);
  mp_s_axis_concat_tready->offset_port(1, 1);
  
  mp_s_axis_split_tdata->out_port[1](s_axis_split_tdata_out_1);
    mp_s_axis_split_tdata->add_mask(1,16,8);
  
  mp_s_axis_split_tlast->out_port[1](s_axis_split_tlast_out_1);
    mp_s_axis_split_tlast->add_mask(1,2,1);

  // initialize socket stubs

}

void axis_switch_0::before_end_of_elaboration()
{
  // configure 'S00_AXIS' transactor

  if (xsc::utils::xsc_sim_manager::getInstanceParameterInt("axis_switch_0", "S00_AXIS_TLM_MODE") != 1)
  {
    // Instantiate Socket Stubs

  // 'S00_AXIS' transactor parameters
    xsc::common_cpp::properties S00_AXIS_transactor_param_props;
    S00_AXIS_transactor_param_props.addLong("TDATA_NUM_BYTES", "1");
    S00_AXIS_transactor_param_props.addLong("TDEST_WIDTH", "0");
    S00_AXIS_transactor_param_props.addLong("TID_WIDTH", "0");
    S00_AXIS_transactor_param_props.addLong("TUSER_WIDTH", "0");
    S00_AXIS_transactor_param_props.addLong("HAS_TREADY", "1");
    S00_AXIS_transactor_param_props.addLong("HAS_TSTRB", "0");
    S00_AXIS_transactor_param_props.addLong("HAS_TKEEP", "0");
    S00_AXIS_transactor_param_props.addLong("HAS_TLAST", "1");
    S00_AXIS_transactor_param_props.addLong("FREQ_HZ", "100000000");
    S00_AXIS_transactor_param_props.addLong("HAS_RESET", "0");
    S00_AXIS_transactor_param_props.addFloat("PHASE", "0.0");
    S00_AXIS_transactor_param_props.addString("CLK_DOMAIN", "");
    S00_AXIS_transactor_param_props.addString("LAYERED_METADATA", "undef");
    S00_AXIS_transactor_param_props.addLong("TSIDE_BAND_1_WIDTH", "0");
    S00_AXIS_transactor_param_props.addLong("TSIDE_BAND_2_WIDTH", "0");

    mp_S00_AXIS_transactor = new xtlm::xaxis_pin2xtlm_t<2,2,2,2,1,1>("S00_AXIS_transactor", S00_AXIS_transactor_param_props);

    // S00_AXIS' transactor ports

    mp_s_axis_tvalid_converter_0 = new xsc::common::vectorN2scalar_converter<2>("s_axis_tvalid_converter_0");
    mp_s_axis_tvalid_converter_0->vector_in(s_axis_split_tvalid_out_0);
    mp_s_axis_tvalid_converter_0->scalar_out(m_s_axis_tvalid_converter_0_signal);
    mp_S00_AXIS_transactor->TVALID(m_s_axis_tvalid_converter_0_signal);
    mp_s_axis_tready_converter_0 = new xsc::common::scalar2vectorN_converter<2>("s_axis_tready_converter_0");
    mp_s_axis_tready_converter_0->scalar_in(m_s_axis_tready_converter_0_signal);
    mp_s_axis_tready_converter_0->vector_out(s_axis_concat_tready_out_0);
    mp_S00_AXIS_transactor->TREADY(m_s_axis_tready_converter_0_signal);
    mp_s_axis_tdata_converter_0 = new xsc::common::vector2vector_converter<16,16>("s_axis_tdata_converter_0");
    mp_s_axis_tdata_converter_0->vector_in(s_axis_split_tdata_out_0);
    mp_s_axis_tdata_converter_0->vector_out(m_s_axis_tdata_converter_0_signal);
    mp_S00_AXIS_transactor->TDATA(m_s_axis_tdata_converter_0_signal);
    mp_s_axis_tlast_converter_0 = new xsc::common::vectorN2scalar_converter<2>("s_axis_tlast_converter_0");
    mp_s_axis_tlast_converter_0->vector_in(s_axis_split_tlast_out_0);
    mp_s_axis_tlast_converter_0->scalar_out(m_s_axis_tlast_converter_0_signal);
    mp_S00_AXIS_transactor->TLAST(m_s_axis_tlast_converter_0_signal);
    mp_S00_AXIS_transactor->CLK(aclk);
    m_S00_AXIS_transactor_rst_signal.write(1);
    mp_S00_AXIS_transactor->RST(m_S00_AXIS_transactor_rst_signal);

    // S00_AXIS' transactor sockets

    mp_impl->S00_AXIS_TARGET_SOCKET->bind(*(mp_S00_AXIS_transactor->socket));
  }
  else
  {
  }

  // configure 'M00_AXIS' transactor

  if (xsc::utils::xsc_sim_manager::getInstanceParameterInt("axis_switch_0", "M00_AXIS_TLM_MODE") != 1)
  {
    // Instantiate Socket Stubs

  // 'M00_AXIS' transactor parameters
    xsc::common_cpp::properties M00_AXIS_transactor_param_props;
    M00_AXIS_transactor_param_props.addLong("TDATA_NUM_BYTES", "1");
    M00_AXIS_transactor_param_props.addLong("TDEST_WIDTH", "0");
    M00_AXIS_transactor_param_props.addLong("TID_WIDTH", "0");
    M00_AXIS_transactor_param_props.addLong("TUSER_WIDTH", "0");
    M00_AXIS_transactor_param_props.addLong("HAS_TREADY", "1");
    M00_AXIS_transactor_param_props.addLong("HAS_TSTRB", "0");
    M00_AXIS_transactor_param_props.addLong("HAS_TKEEP", "0");
    M00_AXIS_transactor_param_props.addLong("HAS_TLAST", "1");
    M00_AXIS_transactor_param_props.addLong("FREQ_HZ", "100000000");
    M00_AXIS_transactor_param_props.addLong("HAS_RESET", "0");
    M00_AXIS_transactor_param_props.addFloat("PHASE", "0.0");
    M00_AXIS_transactor_param_props.addString("CLK_DOMAIN", "");
    M00_AXIS_transactor_param_props.addString("LAYERED_METADATA", "undef");
    M00_AXIS_transactor_param_props.addLong("TSIDE_BAND_1_WIDTH", "0");
    M00_AXIS_transactor_param_props.addLong("TSIDE_BAND_2_WIDTH", "0");

    mp_M00_AXIS_transactor = new xtlm::xaxis_xtlm2pin_t<1,1,1,1,1,1>("M00_AXIS_transactor", M00_AXIS_transactor_param_props);

    // M00_AXIS' transactor ports

    mp_m_axis_tvalid_converter = new xsc::common::scalar2vectorN_converter<1>("m_axis_tvalid_converter");
    mp_m_axis_tvalid_converter->scalar_in(m_m_axis_tvalid_converter_signal);
    mp_m_axis_tvalid_converter->vector_out(m_axis_tvalid);
    mp_M00_AXIS_transactor->TVALID(m_m_axis_tvalid_converter_signal);
    mp_m_axis_tready_converter = new xsc::common::vectorN2scalar_converter<1>("m_axis_tready_converter");
    mp_m_axis_tready_converter->vector_in(m_axis_tready);
    mp_m_axis_tready_converter->scalar_out(m_m_axis_tready_converter_signal);
    mp_M00_AXIS_transactor->TREADY(m_m_axis_tready_converter_signal);
    mp_M00_AXIS_transactor->TDATA(m_axis_tdata);
    mp_m_axis_tlast_converter = new xsc::common::scalar2vectorN_converter<1>("m_axis_tlast_converter");
    mp_m_axis_tlast_converter->scalar_in(m_m_axis_tlast_converter_signal);
    mp_m_axis_tlast_converter->vector_out(m_axis_tlast);
    mp_M00_AXIS_transactor->TLAST(m_m_axis_tlast_converter_signal);
    mp_M00_AXIS_transactor->CLK(aclk);
    m_M00_AXIS_transactor_rst_signal.write(1);
    mp_M00_AXIS_transactor->RST(m_M00_AXIS_transactor_rst_signal);

    // M00_AXIS' transactor sockets

    mp_impl->M00_AXIS_INITIATOR_SOCKET->bind(*(mp_M00_AXIS_transactor->socket));
  }
  else
  {
  }

  // configure 'S01_AXIS' transactor

  if (xsc::utils::xsc_sim_manager::getInstanceParameterInt("axis_switch_0", "S01_AXIS_TLM_MODE") != 1)
  {
    // Instantiate Socket Stubs

  // 'S01_AXIS' transactor parameters
    xsc::common_cpp::properties S01_AXIS_transactor_param_props;
    S01_AXIS_transactor_param_props.addLong("TDATA_NUM_BYTES", "1");
    S01_AXIS_transactor_param_props.addLong("TDEST_WIDTH", "0");
    S01_AXIS_transactor_param_props.addLong("TID_WIDTH", "0");
    S01_AXIS_transactor_param_props.addLong("TUSER_WIDTH", "0");
    S01_AXIS_transactor_param_props.addLong("HAS_TREADY", "1");
    S01_AXIS_transactor_param_props.addLong("HAS_TSTRB", "0");
    S01_AXIS_transactor_param_props.addLong("HAS_TKEEP", "0");
    S01_AXIS_transactor_param_props.addLong("HAS_TLAST", "1");
    S01_AXIS_transactor_param_props.addLong("FREQ_HZ", "100000000");
    S01_AXIS_transactor_param_props.addLong("HAS_RESET", "0");
    S01_AXIS_transactor_param_props.addFloat("PHASE", "0.0");
    S01_AXIS_transactor_param_props.addString("CLK_DOMAIN", "");
    S01_AXIS_transactor_param_props.addString("LAYERED_METADATA", "undef");
    S01_AXIS_transactor_param_props.addLong("TSIDE_BAND_1_WIDTH", "0");
    S01_AXIS_transactor_param_props.addLong("TSIDE_BAND_2_WIDTH", "0");

    mp_S01_AXIS_transactor = new xtlm::xaxis_pin2xtlm_t<2,2,2,2,1,1>("S01_AXIS_transactor", S01_AXIS_transactor_param_props);

    // S01_AXIS' transactor ports

    mp_s_axis_tvalid_converter_1 = new xsc::common::vectorN2scalar_converter<2>("s_axis_tvalid_converter_1");
    mp_s_axis_tvalid_converter_1->vector_in(s_axis_split_tvalid_out_1);
    mp_s_axis_tvalid_converter_1->scalar_out(m_s_axis_tvalid_converter_1_signal);
    mp_S01_AXIS_transactor->TVALID(m_s_axis_tvalid_converter_1_signal);
    mp_s_axis_tready_converter_1 = new xsc::common::scalar2vectorN_converter<2>("s_axis_tready_converter_1");
    mp_s_axis_tready_converter_1->scalar_in(m_s_axis_tready_converter_1_signal);
    mp_s_axis_tready_converter_1->vector_out(s_axis_concat_tready_out_1);
    mp_S01_AXIS_transactor->TREADY(m_s_axis_tready_converter_1_signal);
    mp_s_axis_tdata_converter_1 = new xsc::common::vector2vector_converter<16,16>("s_axis_tdata_converter_1");
    mp_s_axis_tdata_converter_1->vector_in(s_axis_split_tdata_out_1);
    mp_s_axis_tdata_converter_1->vector_out(m_s_axis_tdata_converter_1_signal);
    mp_S01_AXIS_transactor->TDATA(m_s_axis_tdata_converter_1_signal);
    mp_s_axis_tlast_converter_1 = new xsc::common::vectorN2scalar_converter<2>("s_axis_tlast_converter_1");
    mp_s_axis_tlast_converter_1->vector_in(s_axis_split_tlast_out_1);
    mp_s_axis_tlast_converter_1->scalar_out(m_s_axis_tlast_converter_1_signal);
    mp_S01_AXIS_transactor->TLAST(m_s_axis_tlast_converter_1_signal);
    mp_S01_AXIS_transactor->CLK(aclk);
    m_S01_AXIS_transactor_rst_signal.write(1);
    mp_S01_AXIS_transactor->RST(m_S01_AXIS_transactor_rst_signal);

    // S01_AXIS' transactor sockets

    mp_impl->S01_AXIS_TARGET_SOCKET->bind(*(mp_S01_AXIS_transactor->socket));
  }
  else
  {
  }

}

#endif // XILINX_SIMULATOR




#ifdef XM_SYSTEMC
axis_switch_0::axis_switch_0(const sc_core::sc_module_name& nm) : axis_switch_0_sc(nm), aclk("aclk"), aresetn("aresetn"), s_axis_tvalid("s_axis_tvalid"), s_axis_tready("s_axis_tready"), s_axis_tdata("s_axis_tdata"), s_axis_tlast("s_axis_tlast"), m_axis_tvalid("m_axis_tvalid"), m_axis_tready("m_axis_tready"), m_axis_tdata("m_axis_tdata"), m_axis_tlast("m_axis_tlast"), s_req_suppress("s_req_suppress"), s_decode_err("s_decode_err")
{

  // initialize pins
  mp_impl->aclk(aclk);
  mp_impl->aresetn(aresetn);
  mp_impl->s_req_suppress(s_req_suppress);
  mp_impl->s_decode_err(s_decode_err);

  // initialize transactors
  mp_S00_AXIS_transactor = NULL;
  mp_s_axis_tvalid_converter_0 = NULL;
  mp_s_axis_tready_converter_0 = NULL;
  mp_s_axis_tdata_converter_0 = NULL;
  mp_s_axis_tlast_converter_0 = NULL;
  mp_M00_AXIS_transactor = NULL;
  mp_m_axis_tvalid_converter = NULL;
  mp_m_axis_tready_converter = NULL;
  mp_m_axis_tlast_converter = NULL;
  mp_S01_AXIS_transactor = NULL;
  mp_s_axis_tvalid_converter_1 = NULL;
  mp_s_axis_tready_converter_1 = NULL;
  mp_s_axis_tdata_converter_1 = NULL;
  mp_s_axis_tlast_converter_1 = NULL;

  // initialize junctures
  mp_s_axis_concat_tready = NULL;
  mp_s_axis_split_tdata = NULL;
  mp_s_axis_split_tlast = NULL;
  mp_s_axis_split_tvalid = NULL;
  mp_s_axis_split_tvalid = new xsc::xsc_split<2, 2>("s_axis_split_tvalid");
  mp_s_axis_split_tvalid->in_port(s_axis_tvalid);
  mp_s_axis_split_tvalid->out_port[0](s_axis_split_tvalid_out_0);
    mp_s_axis_split_tvalid->add_mask(0,1,0);
  mp_s_axis_concat_tready = new xsc::xsc_concatenator<2, 2>("s_axis_concat_tready");
  mp_s_axis_concat_tready->in_port[0](s_axis_concat_tready_out_0);
  mp_s_axis_concat_tready->out_port(s_axis_tready);
    mp_s_axis_concat_tready->offset_port(0, 0);
  mp_s_axis_split_tdata = new xsc::xsc_split<16, 2>("s_axis_split_tdata");
  mp_s_axis_split_tdata->in_port(s_axis_tdata);
  mp_s_axis_split_tdata->out_port[0](s_axis_split_tdata_out_0);
    mp_s_axis_split_tdata->add_mask(0,8,0);
  mp_s_axis_split_tlast = new xsc::xsc_split<2, 2>("s_axis_split_tlast");
  mp_s_axis_split_tlast->in_port(s_axis_tlast);
  mp_s_axis_split_tlast->out_port[0](s_axis_split_tlast_out_0);
    mp_s_axis_split_tlast->add_mask(0,1,0);
  
  mp_s_axis_split_tvalid->out_port[1](s_axis_split_tvalid_out_1);
    mp_s_axis_split_tvalid->add_mask(1,2,1);
  mp_s_axis_concat_tready->in_port[1](s_axis_concat_tready_out_1);
  mp_s_axis_concat_tready->offset_port(1, 1);
  
  mp_s_axis_split_tdata->out_port[1](s_axis_split_tdata_out_1);
    mp_s_axis_split_tdata->add_mask(1,16,8);
  
  mp_s_axis_split_tlast->out_port[1](s_axis_split_tlast_out_1);
    mp_s_axis_split_tlast->add_mask(1,2,1);

  // initialize socket stubs

}

void axis_switch_0::before_end_of_elaboration()
{
  // configure 'S00_AXIS' transactor

  if (xsc::utils::xsc_sim_manager::getInstanceParameterInt("axis_switch_0", "S00_AXIS_TLM_MODE") != 1)
  {
    // Instantiate Socket Stubs

  // 'S00_AXIS' transactor parameters
    xsc::common_cpp::properties S00_AXIS_transactor_param_props;
    S00_AXIS_transactor_param_props.addLong("TDATA_NUM_BYTES", "1");
    S00_AXIS_transactor_param_props.addLong("TDEST_WIDTH", "0");
    S00_AXIS_transactor_param_props.addLong("TID_WIDTH", "0");
    S00_AXIS_transactor_param_props.addLong("TUSER_WIDTH", "0");
    S00_AXIS_transactor_param_props.addLong("HAS_TREADY", "1");
    S00_AXIS_transactor_param_props.addLong("HAS_TSTRB", "0");
    S00_AXIS_transactor_param_props.addLong("HAS_TKEEP", "0");
    S00_AXIS_transactor_param_props.addLong("HAS_TLAST", "1");
    S00_AXIS_transactor_param_props.addLong("FREQ_HZ", "100000000");
    S00_AXIS_transactor_param_props.addLong("HAS_RESET", "0");
    S00_AXIS_transactor_param_props.addFloat("PHASE", "0.0");
    S00_AXIS_transactor_param_props.addString("CLK_DOMAIN", "");
    S00_AXIS_transactor_param_props.addString("LAYERED_METADATA", "undef");
    S00_AXIS_transactor_param_props.addLong("TSIDE_BAND_1_WIDTH", "0");
    S00_AXIS_transactor_param_props.addLong("TSIDE_BAND_2_WIDTH", "0");

    mp_S00_AXIS_transactor = new xtlm::xaxis_pin2xtlm_t<2,2,2,2,1,1>("S00_AXIS_transactor", S00_AXIS_transactor_param_props);

    // S00_AXIS' transactor ports

    mp_s_axis_tvalid_converter_0 = new xsc::common::vectorN2scalar_converter<2>("s_axis_tvalid_converter_0");
    mp_s_axis_tvalid_converter_0->vector_in(s_axis_split_tvalid_out_0);
    mp_s_axis_tvalid_converter_0->scalar_out(m_s_axis_tvalid_converter_0_signal);
    mp_S00_AXIS_transactor->TVALID(m_s_axis_tvalid_converter_0_signal);
    mp_s_axis_tready_converter_0 = new xsc::common::scalar2vectorN_converter<2>("s_axis_tready_converter_0");
    mp_s_axis_tready_converter_0->scalar_in(m_s_axis_tready_converter_0_signal);
    mp_s_axis_tready_converter_0->vector_out(s_axis_concat_tready_out_0);
    mp_S00_AXIS_transactor->TREADY(m_s_axis_tready_converter_0_signal);
    mp_s_axis_tdata_converter_0 = new xsc::common::vector2vector_converter<16,16>("s_axis_tdata_converter_0");
    mp_s_axis_tdata_converter_0->vector_in(s_axis_split_tdata_out_0);
    mp_s_axis_tdata_converter_0->vector_out(m_s_axis_tdata_converter_0_signal);
    mp_S00_AXIS_transactor->TDATA(m_s_axis_tdata_converter_0_signal);
    mp_s_axis_tlast_converter_0 = new xsc::common::vectorN2scalar_converter<2>("s_axis_tlast_converter_0");
    mp_s_axis_tlast_converter_0->vector_in(s_axis_split_tlast_out_0);
    mp_s_axis_tlast_converter_0->scalar_out(m_s_axis_tlast_converter_0_signal);
    mp_S00_AXIS_transactor->TLAST(m_s_axis_tlast_converter_0_signal);
    mp_S00_AXIS_transactor->CLK(aclk);
    m_S00_AXIS_transactor_rst_signal.write(1);
    mp_S00_AXIS_transactor->RST(m_S00_AXIS_transactor_rst_signal);

    // S00_AXIS' transactor sockets

    mp_impl->S00_AXIS_TARGET_SOCKET->bind(*(mp_S00_AXIS_transactor->socket));
  }
  else
  {
  }

  // configure 'M00_AXIS' transactor

  if (xsc::utils::xsc_sim_manager::getInstanceParameterInt("axis_switch_0", "M00_AXIS_TLM_MODE") != 1)
  {
    // Instantiate Socket Stubs

  // 'M00_AXIS' transactor parameters
    xsc::common_cpp::properties M00_AXIS_transactor_param_props;
    M00_AXIS_transactor_param_props.addLong("TDATA_NUM_BYTES", "1");
    M00_AXIS_transactor_param_props.addLong("TDEST_WIDTH", "0");
    M00_AXIS_transactor_param_props.addLong("TID_WIDTH", "0");
    M00_AXIS_transactor_param_props.addLong("TUSER_WIDTH", "0");
    M00_AXIS_transactor_param_props.addLong("HAS_TREADY", "1");
    M00_AXIS_transactor_param_props.addLong("HAS_TSTRB", "0");
    M00_AXIS_transactor_param_props.addLong("HAS_TKEEP", "0");
    M00_AXIS_transactor_param_props.addLong("HAS_TLAST", "1");
    M00_AXIS_transactor_param_props.addLong("FREQ_HZ", "100000000");
    M00_AXIS_transactor_param_props.addLong("HAS_RESET", "0");
    M00_AXIS_transactor_param_props.addFloat("PHASE", "0.0");
    M00_AXIS_transactor_param_props.addString("CLK_DOMAIN", "");
    M00_AXIS_transactor_param_props.addString("LAYERED_METADATA", "undef");
    M00_AXIS_transactor_param_props.addLong("TSIDE_BAND_1_WIDTH", "0");
    M00_AXIS_transactor_param_props.addLong("TSIDE_BAND_2_WIDTH", "0");

    mp_M00_AXIS_transactor = new xtlm::xaxis_xtlm2pin_t<1,1,1,1,1,1>("M00_AXIS_transactor", M00_AXIS_transactor_param_props);

    // M00_AXIS' transactor ports

    mp_m_axis_tvalid_converter = new xsc::common::scalar2vectorN_converter<1>("m_axis_tvalid_converter");
    mp_m_axis_tvalid_converter->scalar_in(m_m_axis_tvalid_converter_signal);
    mp_m_axis_tvalid_converter->vector_out(m_axis_tvalid);
    mp_M00_AXIS_transactor->TVALID(m_m_axis_tvalid_converter_signal);
    mp_m_axis_tready_converter = new xsc::common::vectorN2scalar_converter<1>("m_axis_tready_converter");
    mp_m_axis_tready_converter->vector_in(m_axis_tready);
    mp_m_axis_tready_converter->scalar_out(m_m_axis_tready_converter_signal);
    mp_M00_AXIS_transactor->TREADY(m_m_axis_tready_converter_signal);
    mp_M00_AXIS_transactor->TDATA(m_axis_tdata);
    mp_m_axis_tlast_converter = new xsc::common::scalar2vectorN_converter<1>("m_axis_tlast_converter");
    mp_m_axis_tlast_converter->scalar_in(m_m_axis_tlast_converter_signal);
    mp_m_axis_tlast_converter->vector_out(m_axis_tlast);
    mp_M00_AXIS_transactor->TLAST(m_m_axis_tlast_converter_signal);
    mp_M00_AXIS_transactor->CLK(aclk);
    m_M00_AXIS_transactor_rst_signal.write(1);
    mp_M00_AXIS_transactor->RST(m_M00_AXIS_transactor_rst_signal);

    // M00_AXIS' transactor sockets

    mp_impl->M00_AXIS_INITIATOR_SOCKET->bind(*(mp_M00_AXIS_transactor->socket));
  }
  else
  {
  }

  // configure 'S01_AXIS' transactor

  if (xsc::utils::xsc_sim_manager::getInstanceParameterInt("axis_switch_0", "S01_AXIS_TLM_MODE") != 1)
  {
    // Instantiate Socket Stubs

  // 'S01_AXIS' transactor parameters
    xsc::common_cpp::properties S01_AXIS_transactor_param_props;
    S01_AXIS_transactor_param_props.addLong("TDATA_NUM_BYTES", "1");
    S01_AXIS_transactor_param_props.addLong("TDEST_WIDTH", "0");
    S01_AXIS_transactor_param_props.addLong("TID_WIDTH", "0");
    S01_AXIS_transactor_param_props.addLong("TUSER_WIDTH", "0");
    S01_AXIS_transactor_param_props.addLong("HAS_TREADY", "1");
    S01_AXIS_transactor_param_props.addLong("HAS_TSTRB", "0");
    S01_AXIS_transactor_param_props.addLong("HAS_TKEEP", "0");
    S01_AXIS_transactor_param_props.addLong("HAS_TLAST", "1");
    S01_AXIS_transactor_param_props.addLong("FREQ_HZ", "100000000");
    S01_AXIS_transactor_param_props.addLong("HAS_RESET", "0");
    S01_AXIS_transactor_param_props.addFloat("PHASE", "0.0");
    S01_AXIS_transactor_param_props.addString("CLK_DOMAIN", "");
    S01_AXIS_transactor_param_props.addString("LAYERED_METADATA", "undef");
    S01_AXIS_transactor_param_props.addLong("TSIDE_BAND_1_WIDTH", "0");
    S01_AXIS_transactor_param_props.addLong("TSIDE_BAND_2_WIDTH", "0");

    mp_S01_AXIS_transactor = new xtlm::xaxis_pin2xtlm_t<2,2,2,2,1,1>("S01_AXIS_transactor", S01_AXIS_transactor_param_props);

    // S01_AXIS' transactor ports

    mp_s_axis_tvalid_converter_1 = new xsc::common::vectorN2scalar_converter<2>("s_axis_tvalid_converter_1");
    mp_s_axis_tvalid_converter_1->vector_in(s_axis_split_tvalid_out_1);
    mp_s_axis_tvalid_converter_1->scalar_out(m_s_axis_tvalid_converter_1_signal);
    mp_S01_AXIS_transactor->TVALID(m_s_axis_tvalid_converter_1_signal);
    mp_s_axis_tready_converter_1 = new xsc::common::scalar2vectorN_converter<2>("s_axis_tready_converter_1");
    mp_s_axis_tready_converter_1->scalar_in(m_s_axis_tready_converter_1_signal);
    mp_s_axis_tready_converter_1->vector_out(s_axis_concat_tready_out_1);
    mp_S01_AXIS_transactor->TREADY(m_s_axis_tready_converter_1_signal);
    mp_s_axis_tdata_converter_1 = new xsc::common::vector2vector_converter<16,16>("s_axis_tdata_converter_1");
    mp_s_axis_tdata_converter_1->vector_in(s_axis_split_tdata_out_1);
    mp_s_axis_tdata_converter_1->vector_out(m_s_axis_tdata_converter_1_signal);
    mp_S01_AXIS_transactor->TDATA(m_s_axis_tdata_converter_1_signal);
    mp_s_axis_tlast_converter_1 = new xsc::common::vectorN2scalar_converter<2>("s_axis_tlast_converter_1");
    mp_s_axis_tlast_converter_1->vector_in(s_axis_split_tlast_out_1);
    mp_s_axis_tlast_converter_1->scalar_out(m_s_axis_tlast_converter_1_signal);
    mp_S01_AXIS_transactor->TLAST(m_s_axis_tlast_converter_1_signal);
    mp_S01_AXIS_transactor->CLK(aclk);
    m_S01_AXIS_transactor_rst_signal.write(1);
    mp_S01_AXIS_transactor->RST(m_S01_AXIS_transactor_rst_signal);

    // S01_AXIS' transactor sockets

    mp_impl->S01_AXIS_TARGET_SOCKET->bind(*(mp_S01_AXIS_transactor->socket));
  }
  else
  {
  }

}

#endif // XM_SYSTEMC




#ifdef RIVIERA
axis_switch_0::axis_switch_0(const sc_core::sc_module_name& nm) : axis_switch_0_sc(nm), aclk("aclk"), aresetn("aresetn"), s_axis_tvalid("s_axis_tvalid"), s_axis_tready("s_axis_tready"), s_axis_tdata("s_axis_tdata"), s_axis_tlast("s_axis_tlast"), m_axis_tvalid("m_axis_tvalid"), m_axis_tready("m_axis_tready"), m_axis_tdata("m_axis_tdata"), m_axis_tlast("m_axis_tlast"), s_req_suppress("s_req_suppress"), s_decode_err("s_decode_err")
{

  // initialize pins
  mp_impl->aclk(aclk);
  mp_impl->aresetn(aresetn);
  mp_impl->s_req_suppress(s_req_suppress);
  mp_impl->s_decode_err(s_decode_err);

  // initialize transactors
  mp_S00_AXIS_transactor = NULL;
  mp_s_axis_tvalid_converter_0 = NULL;
  mp_s_axis_tready_converter_0 = NULL;
  mp_s_axis_tdata_converter_0 = NULL;
  mp_s_axis_tlast_converter_0 = NULL;
  mp_M00_AXIS_transactor = NULL;
  mp_m_axis_tvalid_converter = NULL;
  mp_m_axis_tready_converter = NULL;
  mp_m_axis_tlast_converter = NULL;
  mp_S01_AXIS_transactor = NULL;
  mp_s_axis_tvalid_converter_1 = NULL;
  mp_s_axis_tready_converter_1 = NULL;
  mp_s_axis_tdata_converter_1 = NULL;
  mp_s_axis_tlast_converter_1 = NULL;

  // initialize junctures
  mp_s_axis_concat_tready = NULL;
  mp_s_axis_split_tdata = NULL;
  mp_s_axis_split_tlast = NULL;
  mp_s_axis_split_tvalid = NULL;
  mp_s_axis_split_tvalid = new xsc::xsc_split<2, 2>("s_axis_split_tvalid");
  mp_s_axis_split_tvalid->in_port(s_axis_tvalid);
  mp_s_axis_split_tvalid->out_port[0](s_axis_split_tvalid_out_0);
    mp_s_axis_split_tvalid->add_mask(0,1,0);
  mp_s_axis_concat_tready = new xsc::xsc_concatenator<2, 2>("s_axis_concat_tready");
  mp_s_axis_concat_tready->in_port[0](s_axis_concat_tready_out_0);
  mp_s_axis_concat_tready->out_port(s_axis_tready);
    mp_s_axis_concat_tready->offset_port(0, 0);
  mp_s_axis_split_tdata = new xsc::xsc_split<16, 2>("s_axis_split_tdata");
  mp_s_axis_split_tdata->in_port(s_axis_tdata);
  mp_s_axis_split_tdata->out_port[0](s_axis_split_tdata_out_0);
    mp_s_axis_split_tdata->add_mask(0,8,0);
  mp_s_axis_split_tlast = new xsc::xsc_split<2, 2>("s_axis_split_tlast");
  mp_s_axis_split_tlast->in_port(s_axis_tlast);
  mp_s_axis_split_tlast->out_port[0](s_axis_split_tlast_out_0);
    mp_s_axis_split_tlast->add_mask(0,1,0);
  
  mp_s_axis_split_tvalid->out_port[1](s_axis_split_tvalid_out_1);
    mp_s_axis_split_tvalid->add_mask(1,2,1);
  mp_s_axis_concat_tready->in_port[1](s_axis_concat_tready_out_1);
  mp_s_axis_concat_tready->offset_port(1, 1);
  
  mp_s_axis_split_tdata->out_port[1](s_axis_split_tdata_out_1);
    mp_s_axis_split_tdata->add_mask(1,16,8);
  
  mp_s_axis_split_tlast->out_port[1](s_axis_split_tlast_out_1);
    mp_s_axis_split_tlast->add_mask(1,2,1);

  // initialize socket stubs

}

void axis_switch_0::before_end_of_elaboration()
{
  // configure 'S00_AXIS' transactor

  if (xsc::utils::xsc_sim_manager::getInstanceParameterInt("axis_switch_0", "S00_AXIS_TLM_MODE") != 1)
  {
    // Instantiate Socket Stubs

  // 'S00_AXIS' transactor parameters
    xsc::common_cpp::properties S00_AXIS_transactor_param_props;
    S00_AXIS_transactor_param_props.addLong("TDATA_NUM_BYTES", "1");
    S00_AXIS_transactor_param_props.addLong("TDEST_WIDTH", "0");
    S00_AXIS_transactor_param_props.addLong("TID_WIDTH", "0");
    S00_AXIS_transactor_param_props.addLong("TUSER_WIDTH", "0");
    S00_AXIS_transactor_param_props.addLong("HAS_TREADY", "1");
    S00_AXIS_transactor_param_props.addLong("HAS_TSTRB", "0");
    S00_AXIS_transactor_param_props.addLong("HAS_TKEEP", "0");
    S00_AXIS_transactor_param_props.addLong("HAS_TLAST", "1");
    S00_AXIS_transactor_param_props.addLong("FREQ_HZ", "100000000");
    S00_AXIS_transactor_param_props.addLong("HAS_RESET", "0");
    S00_AXIS_transactor_param_props.addFloat("PHASE", "0.0");
    S00_AXIS_transactor_param_props.addString("CLK_DOMAIN", "");
    S00_AXIS_transactor_param_props.addString("LAYERED_METADATA", "undef");
    S00_AXIS_transactor_param_props.addLong("TSIDE_BAND_1_WIDTH", "0");
    S00_AXIS_transactor_param_props.addLong("TSIDE_BAND_2_WIDTH", "0");

    mp_S00_AXIS_transactor = new xtlm::xaxis_pin2xtlm_t<2,2,2,2,1,1>("S00_AXIS_transactor", S00_AXIS_transactor_param_props);

    // S00_AXIS' transactor ports

    mp_s_axis_tvalid_converter_0 = new xsc::common::vectorN2scalar_converter<2>("s_axis_tvalid_converter_0");
    mp_s_axis_tvalid_converter_0->vector_in(s_axis_split_tvalid_out_0);
    mp_s_axis_tvalid_converter_0->scalar_out(m_s_axis_tvalid_converter_0_signal);
    mp_S00_AXIS_transactor->TVALID(m_s_axis_tvalid_converter_0_signal);
    mp_s_axis_tready_converter_0 = new xsc::common::scalar2vectorN_converter<2>("s_axis_tready_converter_0");
    mp_s_axis_tready_converter_0->scalar_in(m_s_axis_tready_converter_0_signal);
    mp_s_axis_tready_converter_0->vector_out(s_axis_concat_tready_out_0);
    mp_S00_AXIS_transactor->TREADY(m_s_axis_tready_converter_0_signal);
    mp_s_axis_tdata_converter_0 = new xsc::common::vector2vector_converter<16,16>("s_axis_tdata_converter_0");
    mp_s_axis_tdata_converter_0->vector_in(s_axis_split_tdata_out_0);
    mp_s_axis_tdata_converter_0->vector_out(m_s_axis_tdata_converter_0_signal);
    mp_S00_AXIS_transactor->TDATA(m_s_axis_tdata_converter_0_signal);
    mp_s_axis_tlast_converter_0 = new xsc::common::vectorN2scalar_converter<2>("s_axis_tlast_converter_0");
    mp_s_axis_tlast_converter_0->vector_in(s_axis_split_tlast_out_0);
    mp_s_axis_tlast_converter_0->scalar_out(m_s_axis_tlast_converter_0_signal);
    mp_S00_AXIS_transactor->TLAST(m_s_axis_tlast_converter_0_signal);
    mp_S00_AXIS_transactor->CLK(aclk);
    m_S00_AXIS_transactor_rst_signal.write(1);
    mp_S00_AXIS_transactor->RST(m_S00_AXIS_transactor_rst_signal);

    // S00_AXIS' transactor sockets

    mp_impl->S00_AXIS_TARGET_SOCKET->bind(*(mp_S00_AXIS_transactor->socket));
  }
  else
  {
  }

  // configure 'M00_AXIS' transactor

  if (xsc::utils::xsc_sim_manager::getInstanceParameterInt("axis_switch_0", "M00_AXIS_TLM_MODE") != 1)
  {
    // Instantiate Socket Stubs

  // 'M00_AXIS' transactor parameters
    xsc::common_cpp::properties M00_AXIS_transactor_param_props;
    M00_AXIS_transactor_param_props.addLong("TDATA_NUM_BYTES", "1");
    M00_AXIS_transactor_param_props.addLong("TDEST_WIDTH", "0");
    M00_AXIS_transactor_param_props.addLong("TID_WIDTH", "0");
    M00_AXIS_transactor_param_props.addLong("TUSER_WIDTH", "0");
    M00_AXIS_transactor_param_props.addLong("HAS_TREADY", "1");
    M00_AXIS_transactor_param_props.addLong("HAS_TSTRB", "0");
    M00_AXIS_transactor_param_props.addLong("HAS_TKEEP", "0");
    M00_AXIS_transactor_param_props.addLong("HAS_TLAST", "1");
    M00_AXIS_transactor_param_props.addLong("FREQ_HZ", "100000000");
    M00_AXIS_transactor_param_props.addLong("HAS_RESET", "0");
    M00_AXIS_transactor_param_props.addFloat("PHASE", "0.0");
    M00_AXIS_transactor_param_props.addString("CLK_DOMAIN", "");
    M00_AXIS_transactor_param_props.addString("LAYERED_METADATA", "undef");
    M00_AXIS_transactor_param_props.addLong("TSIDE_BAND_1_WIDTH", "0");
    M00_AXIS_transactor_param_props.addLong("TSIDE_BAND_2_WIDTH", "0");

    mp_M00_AXIS_transactor = new xtlm::xaxis_xtlm2pin_t<1,1,1,1,1,1>("M00_AXIS_transactor", M00_AXIS_transactor_param_props);

    // M00_AXIS' transactor ports

    mp_m_axis_tvalid_converter = new xsc::common::scalar2vectorN_converter<1>("m_axis_tvalid_converter");
    mp_m_axis_tvalid_converter->scalar_in(m_m_axis_tvalid_converter_signal);
    mp_m_axis_tvalid_converter->vector_out(m_axis_tvalid);
    mp_M00_AXIS_transactor->TVALID(m_m_axis_tvalid_converter_signal);
    mp_m_axis_tready_converter = new xsc::common::vectorN2scalar_converter<1>("m_axis_tready_converter");
    mp_m_axis_tready_converter->vector_in(m_axis_tready);
    mp_m_axis_tready_converter->scalar_out(m_m_axis_tready_converter_signal);
    mp_M00_AXIS_transactor->TREADY(m_m_axis_tready_converter_signal);
    mp_M00_AXIS_transactor->TDATA(m_axis_tdata);
    mp_m_axis_tlast_converter = new xsc::common::scalar2vectorN_converter<1>("m_axis_tlast_converter");
    mp_m_axis_tlast_converter->scalar_in(m_m_axis_tlast_converter_signal);
    mp_m_axis_tlast_converter->vector_out(m_axis_tlast);
    mp_M00_AXIS_transactor->TLAST(m_m_axis_tlast_converter_signal);
    mp_M00_AXIS_transactor->CLK(aclk);
    m_M00_AXIS_transactor_rst_signal.write(1);
    mp_M00_AXIS_transactor->RST(m_M00_AXIS_transactor_rst_signal);

    // M00_AXIS' transactor sockets

    mp_impl->M00_AXIS_INITIATOR_SOCKET->bind(*(mp_M00_AXIS_transactor->socket));
  }
  else
  {
  }

  // configure 'S01_AXIS' transactor

  if (xsc::utils::xsc_sim_manager::getInstanceParameterInt("axis_switch_0", "S01_AXIS_TLM_MODE") != 1)
  {
    // Instantiate Socket Stubs

  // 'S01_AXIS' transactor parameters
    xsc::common_cpp::properties S01_AXIS_transactor_param_props;
    S01_AXIS_transactor_param_props.addLong("TDATA_NUM_BYTES", "1");
    S01_AXIS_transactor_param_props.addLong("TDEST_WIDTH", "0");
    S01_AXIS_transactor_param_props.addLong("TID_WIDTH", "0");
    S01_AXIS_transactor_param_props.addLong("TUSER_WIDTH", "0");
    S01_AXIS_transactor_param_props.addLong("HAS_TREADY", "1");
    S01_AXIS_transactor_param_props.addLong("HAS_TSTRB", "0");
    S01_AXIS_transactor_param_props.addLong("HAS_TKEEP", "0");
    S01_AXIS_transactor_param_props.addLong("HAS_TLAST", "1");
    S01_AXIS_transactor_param_props.addLong("FREQ_HZ", "100000000");
    S01_AXIS_transactor_param_props.addLong("HAS_RESET", "0");
    S01_AXIS_transactor_param_props.addFloat("PHASE", "0.0");
    S01_AXIS_transactor_param_props.addString("CLK_DOMAIN", "");
    S01_AXIS_transactor_param_props.addString("LAYERED_METADATA", "undef");
    S01_AXIS_transactor_param_props.addLong("TSIDE_BAND_1_WIDTH", "0");
    S01_AXIS_transactor_param_props.addLong("TSIDE_BAND_2_WIDTH", "0");

    mp_S01_AXIS_transactor = new xtlm::xaxis_pin2xtlm_t<2,2,2,2,1,1>("S01_AXIS_transactor", S01_AXIS_transactor_param_props);

    // S01_AXIS' transactor ports

    mp_s_axis_tvalid_converter_1 = new xsc::common::vectorN2scalar_converter<2>("s_axis_tvalid_converter_1");
    mp_s_axis_tvalid_converter_1->vector_in(s_axis_split_tvalid_out_1);
    mp_s_axis_tvalid_converter_1->scalar_out(m_s_axis_tvalid_converter_1_signal);
    mp_S01_AXIS_transactor->TVALID(m_s_axis_tvalid_converter_1_signal);
    mp_s_axis_tready_converter_1 = new xsc::common::scalar2vectorN_converter<2>("s_axis_tready_converter_1");
    mp_s_axis_tready_converter_1->scalar_in(m_s_axis_tready_converter_1_signal);
    mp_s_axis_tready_converter_1->vector_out(s_axis_concat_tready_out_1);
    mp_S01_AXIS_transactor->TREADY(m_s_axis_tready_converter_1_signal);
    mp_s_axis_tdata_converter_1 = new xsc::common::vector2vector_converter<16,16>("s_axis_tdata_converter_1");
    mp_s_axis_tdata_converter_1->vector_in(s_axis_split_tdata_out_1);
    mp_s_axis_tdata_converter_1->vector_out(m_s_axis_tdata_converter_1_signal);
    mp_S01_AXIS_transactor->TDATA(m_s_axis_tdata_converter_1_signal);
    mp_s_axis_tlast_converter_1 = new xsc::common::vectorN2scalar_converter<2>("s_axis_tlast_converter_1");
    mp_s_axis_tlast_converter_1->vector_in(s_axis_split_tlast_out_1);
    mp_s_axis_tlast_converter_1->scalar_out(m_s_axis_tlast_converter_1_signal);
    mp_S01_AXIS_transactor->TLAST(m_s_axis_tlast_converter_1_signal);
    mp_S01_AXIS_transactor->CLK(aclk);
    m_S01_AXIS_transactor_rst_signal.write(1);
    mp_S01_AXIS_transactor->RST(m_S01_AXIS_transactor_rst_signal);

    // S01_AXIS' transactor sockets

    mp_impl->S01_AXIS_TARGET_SOCKET->bind(*(mp_S01_AXIS_transactor->socket));
  }
  else
  {
  }

}

#endif // RIVIERA




#ifdef VCSSYSTEMC
axis_switch_0::axis_switch_0(const sc_core::sc_module_name& nm) : axis_switch_0_sc(nm),  aclk("aclk"), aresetn("aresetn"), s_axis_tvalid("s_axis_tvalid"), s_axis_tready("s_axis_tready"), s_axis_tdata("s_axis_tdata"), s_axis_tlast("s_axis_tlast"), m_axis_tvalid("m_axis_tvalid"), m_axis_tready("m_axis_tready"), m_axis_tdata("m_axis_tdata"), m_axis_tlast("m_axis_tlast"), s_req_suppress("s_req_suppress"), s_decode_err("s_decode_err")
{
  // initialize pins
  mp_impl->aclk(aclk);
  mp_impl->aresetn(aresetn);
  mp_impl->s_req_suppress(s_req_suppress);
  mp_impl->s_decode_err(s_decode_err);

  // initialize transactors
  mp_S00_AXIS_transactor = NULL;
  mp_s_axis_tvalid_converter_0 = NULL;
  mp_s_axis_tready_converter_0 = NULL;
  mp_s_axis_tdata_converter_0 = NULL;
  mp_s_axis_tlast_converter_0 = NULL;
  mp_M00_AXIS_transactor = NULL;
  mp_m_axis_tvalid_converter = NULL;
  mp_m_axis_tready_converter = NULL;
  mp_m_axis_tlast_converter = NULL;
  mp_S01_AXIS_transactor = NULL;
  mp_s_axis_tvalid_converter_1 = NULL;
  mp_s_axis_tready_converter_1 = NULL;
  mp_s_axis_tdata_converter_1 = NULL;
  mp_s_axis_tlast_converter_1 = NULL;

  // initialize port junctures
  mp_s_axis_concat_tready = NULL;
  mp_s_axis_split_tdata = NULL;
  mp_s_axis_split_tlast = NULL;
  mp_s_axis_split_tvalid = NULL;

  // Instantiate Socket Stubs

  // configure S00_AXIS_transactor
    xsc::common_cpp::properties S00_AXIS_transactor_param_props;
    S00_AXIS_transactor_param_props.addLong("TDATA_NUM_BYTES", "1");
    S00_AXIS_transactor_param_props.addLong("TDEST_WIDTH", "0");
    S00_AXIS_transactor_param_props.addLong("TID_WIDTH", "0");
    S00_AXIS_transactor_param_props.addLong("TUSER_WIDTH", "0");
    S00_AXIS_transactor_param_props.addLong("HAS_TREADY", "1");
    S00_AXIS_transactor_param_props.addLong("HAS_TSTRB", "0");
    S00_AXIS_transactor_param_props.addLong("HAS_TKEEP", "0");
    S00_AXIS_transactor_param_props.addLong("HAS_TLAST", "1");
    S00_AXIS_transactor_param_props.addLong("FREQ_HZ", "100000000");
    S00_AXIS_transactor_param_props.addLong("HAS_RESET", "0");
    S00_AXIS_transactor_param_props.addFloat("PHASE", "0.0");
    S00_AXIS_transactor_param_props.addString("CLK_DOMAIN", "");
    S00_AXIS_transactor_param_props.addString("LAYERED_METADATA", "undef");
    S00_AXIS_transactor_param_props.addLong("TSIDE_BAND_1_WIDTH", "0");
    S00_AXIS_transactor_param_props.addLong("TSIDE_BAND_2_WIDTH", "0");

    mp_S00_AXIS_transactor = new xtlm::xaxis_pin2xtlm_t<2,2,2,2,1,1>("S00_AXIS_transactor", S00_AXIS_transactor_param_props);
  mp_s_axis_tvalid_converter_0 = new xsc::common::vectorN2scalar_converter<2>("s_axis_tvalid_converter_0");
  mp_s_axis_split_tvalid = new xsc::xsc_split<2, 2>("s_axis_split_tvalid");
  mp_s_axis_split_tvalid->in_port(s_axis_tvalid);
  mp_s_axis_split_tvalid->out_port[0](s_axis_split_tvalid_out_0);
    mp_s_axis_split_tvalid->add_mask(0,1,0);
  mp_s_axis_tvalid_converter_0->vector_in(s_axis_split_tvalid_out_0);
  mp_s_axis_tvalid_converter_0->scalar_out(m_s_axis_tvalid_converter_0_signal);
  mp_S00_AXIS_transactor->TVALID(m_s_axis_tvalid_converter_0_signal);
  mp_s_axis_tready_converter_0 = new xsc::common::scalar2vectorN_converter<2>("s_axis_tready_converter_0");
  mp_s_axis_concat_tready = new xsc::xsc_concatenator<2, 2>("s_axis_concat_tready");
  mp_s_axis_concat_tready->in_port[0](s_axis_concat_tready_out_0);
  mp_s_axis_concat_tready->out_port(s_axis_tready);
    mp_s_axis_concat_tready->offset_port(0, 0);
  mp_s_axis_tready_converter_0->scalar_in(m_s_axis_tready_converter_0_signal);
  mp_s_axis_tready_converter_0->vector_out(s_axis_concat_tready_out_0);
  mp_S00_AXIS_transactor->TREADY(m_s_axis_tready_converter_0_signal);
  mp_s_axis_tdata_converter_0 = new xsc::common::vector2vector_converter<16,16>("s_axis_tdata_converter_0");
  mp_s_axis_split_tdata = new xsc::xsc_split<16, 2>("s_axis_split_tdata");
  mp_s_axis_split_tdata->in_port(s_axis_tdata);
  mp_s_axis_split_tdata->out_port[0](s_axis_split_tdata_out_0);
    mp_s_axis_split_tdata->add_mask(0,8,0);
  mp_s_axis_tdata_converter_0->vector_in(s_axis_split_tdata_out_0);
  mp_s_axis_tdata_converter_0->vector_out(m_s_axis_tdata_converter_0_signal);
  mp_S00_AXIS_transactor->TDATA(m_s_axis_tdata_converter_0_signal);
  mp_s_axis_tlast_converter_0 = new xsc::common::vectorN2scalar_converter<2>("s_axis_tlast_converter_0");
  mp_s_axis_split_tlast = new xsc::xsc_split<2, 2>("s_axis_split_tlast");
  mp_s_axis_split_tlast->in_port(s_axis_tlast);
  mp_s_axis_split_tlast->out_port[0](s_axis_split_tlast_out_0);
    mp_s_axis_split_tlast->add_mask(0,1,0);
  mp_s_axis_tlast_converter_0->vector_in(s_axis_split_tlast_out_0);
  mp_s_axis_tlast_converter_0->scalar_out(m_s_axis_tlast_converter_0_signal);
  mp_S00_AXIS_transactor->TLAST(m_s_axis_tlast_converter_0_signal);
  mp_S00_AXIS_transactor->CLK(aclk);
  m_S00_AXIS_transactor_rst_signal.write(1);
  mp_S00_AXIS_transactor->RST(m_S00_AXIS_transactor_rst_signal);
  // configure M00_AXIS_transactor
    xsc::common_cpp::properties M00_AXIS_transactor_param_props;
    M00_AXIS_transactor_param_props.addLong("TDATA_NUM_BYTES", "1");
    M00_AXIS_transactor_param_props.addLong("TDEST_WIDTH", "0");
    M00_AXIS_transactor_param_props.addLong("TID_WIDTH", "0");
    M00_AXIS_transactor_param_props.addLong("TUSER_WIDTH", "0");
    M00_AXIS_transactor_param_props.addLong("HAS_TREADY", "1");
    M00_AXIS_transactor_param_props.addLong("HAS_TSTRB", "0");
    M00_AXIS_transactor_param_props.addLong("HAS_TKEEP", "0");
    M00_AXIS_transactor_param_props.addLong("HAS_TLAST", "1");
    M00_AXIS_transactor_param_props.addLong("FREQ_HZ", "100000000");
    M00_AXIS_transactor_param_props.addLong("HAS_RESET", "0");
    M00_AXIS_transactor_param_props.addFloat("PHASE", "0.0");
    M00_AXIS_transactor_param_props.addString("CLK_DOMAIN", "");
    M00_AXIS_transactor_param_props.addString("LAYERED_METADATA", "undef");
    M00_AXIS_transactor_param_props.addLong("TSIDE_BAND_1_WIDTH", "0");
    M00_AXIS_transactor_param_props.addLong("TSIDE_BAND_2_WIDTH", "0");

    mp_M00_AXIS_transactor = new xtlm::xaxis_xtlm2pin_t<1,1,1,1,1,1>("M00_AXIS_transactor", M00_AXIS_transactor_param_props);
  mp_m_axis_tvalid_converter = new xsc::common::scalar2vectorN_converter<1>("m_axis_tvalid_converter");
  mp_m_axis_tvalid_converter->scalar_in(m_m_axis_tvalid_converter_signal);
  mp_m_axis_tvalid_converter->vector_out(m_axis_tvalid);
  mp_M00_AXIS_transactor->TVALID(m_m_axis_tvalid_converter_signal);
  mp_m_axis_tready_converter = new xsc::common::vectorN2scalar_converter<1>("m_axis_tready_converter");
  mp_m_axis_tready_converter->vector_in(m_axis_tready);
  mp_m_axis_tready_converter->scalar_out(m_m_axis_tready_converter_signal);
  mp_M00_AXIS_transactor->TREADY(m_m_axis_tready_converter_signal);
  mp_M00_AXIS_transactor->TDATA(m_axis_tdata);
  mp_m_axis_tlast_converter = new xsc::common::scalar2vectorN_converter<1>("m_axis_tlast_converter");
  mp_m_axis_tlast_converter->scalar_in(m_m_axis_tlast_converter_signal);
  mp_m_axis_tlast_converter->vector_out(m_axis_tlast);
  mp_M00_AXIS_transactor->TLAST(m_m_axis_tlast_converter_signal);
  mp_M00_AXIS_transactor->CLK(aclk);
  m_M00_AXIS_transactor_rst_signal.write(1);
  mp_M00_AXIS_transactor->RST(m_M00_AXIS_transactor_rst_signal);
  // configure S01_AXIS_transactor
    xsc::common_cpp::properties S01_AXIS_transactor_param_props;
    S01_AXIS_transactor_param_props.addLong("TDATA_NUM_BYTES", "1");
    S01_AXIS_transactor_param_props.addLong("TDEST_WIDTH", "0");
    S01_AXIS_transactor_param_props.addLong("TID_WIDTH", "0");
    S01_AXIS_transactor_param_props.addLong("TUSER_WIDTH", "0");
    S01_AXIS_transactor_param_props.addLong("HAS_TREADY", "1");
    S01_AXIS_transactor_param_props.addLong("HAS_TSTRB", "0");
    S01_AXIS_transactor_param_props.addLong("HAS_TKEEP", "0");
    S01_AXIS_transactor_param_props.addLong("HAS_TLAST", "1");
    S01_AXIS_transactor_param_props.addLong("FREQ_HZ", "100000000");
    S01_AXIS_transactor_param_props.addLong("HAS_RESET", "0");
    S01_AXIS_transactor_param_props.addFloat("PHASE", "0.0");
    S01_AXIS_transactor_param_props.addString("CLK_DOMAIN", "");
    S01_AXIS_transactor_param_props.addString("LAYERED_METADATA", "undef");
    S01_AXIS_transactor_param_props.addLong("TSIDE_BAND_1_WIDTH", "0");
    S01_AXIS_transactor_param_props.addLong("TSIDE_BAND_2_WIDTH", "0");

    mp_S01_AXIS_transactor = new xtlm::xaxis_pin2xtlm_t<2,2,2,2,1,1>("S01_AXIS_transactor", S01_AXIS_transactor_param_props);
  mp_s_axis_tvalid_converter_1 = new xsc::common::vectorN2scalar_converter<2>("s_axis_tvalid_converter_1");
  
  mp_s_axis_split_tvalid->out_port[1](s_axis_split_tvalid_out_1);
    mp_s_axis_split_tvalid->add_mask(1,2,1);
  mp_s_axis_tvalid_converter_1->vector_in(s_axis_split_tvalid_out_1);
  mp_s_axis_tvalid_converter_1->scalar_out(m_s_axis_tvalid_converter_1_signal);
  mp_S01_AXIS_transactor->TVALID(m_s_axis_tvalid_converter_1_signal);
  mp_s_axis_tready_converter_1 = new xsc::common::scalar2vectorN_converter<2>("s_axis_tready_converter_1");
  mp_s_axis_concat_tready->in_port[1](s_axis_concat_tready_out_1);
  mp_s_axis_concat_tready->offset_port(1, 1);
  mp_s_axis_tready_converter_1->scalar_in(m_s_axis_tready_converter_1_signal);
  mp_s_axis_tready_converter_1->vector_out(s_axis_concat_tready_out_1);
  mp_S01_AXIS_transactor->TREADY(m_s_axis_tready_converter_1_signal);
  mp_s_axis_tdata_converter_1 = new xsc::common::vector2vector_converter<16,16>("s_axis_tdata_converter_1");
  
  mp_s_axis_split_tdata->out_port[1](s_axis_split_tdata_out_1);
    mp_s_axis_split_tdata->add_mask(1,16,8);
  mp_s_axis_tdata_converter_1->vector_in(s_axis_split_tdata_out_1);
  mp_s_axis_tdata_converter_1->vector_out(m_s_axis_tdata_converter_1_signal);
  mp_S01_AXIS_transactor->TDATA(m_s_axis_tdata_converter_1_signal);
  mp_s_axis_tlast_converter_1 = new xsc::common::vectorN2scalar_converter<2>("s_axis_tlast_converter_1");
  
  mp_s_axis_split_tlast->out_port[1](s_axis_split_tlast_out_1);
    mp_s_axis_split_tlast->add_mask(1,2,1);
  mp_s_axis_tlast_converter_1->vector_in(s_axis_split_tlast_out_1);
  mp_s_axis_tlast_converter_1->scalar_out(m_s_axis_tlast_converter_1_signal);
  mp_S01_AXIS_transactor->TLAST(m_s_axis_tlast_converter_1_signal);
  mp_S01_AXIS_transactor->CLK(aclk);
  m_S01_AXIS_transactor_rst_signal.write(1);
  mp_S01_AXIS_transactor->RST(m_S01_AXIS_transactor_rst_signal);

  // initialize transactors stubs
  S00_AXIS_transactor_target_socket_stub = nullptr;
  M00_AXIS_transactor_initiator_socket_stub = nullptr;
  S01_AXIS_transactor_target_socket_stub = nullptr;

}

void axis_switch_0::before_end_of_elaboration()
{
  // configure 'S00_AXIS' transactor
  if (xsc::utils::xsc_sim_manager::getInstanceParameterInt("axis_switch_0", "S00_AXIS_TLM_MODE") != 1)
  {
    mp_impl->S00_AXIS_TARGET_SOCKET->bind(*(mp_S00_AXIS_transactor->socket));
  
  }
  else
  {
    S00_AXIS_transactor_target_socket_stub = new xtlm::xtlm_axis_target_stub("socket",0);
    S00_AXIS_transactor_target_socket_stub->bind(*(mp_S00_AXIS_transactor->socket));
    mp_S00_AXIS_transactor->disable_transactor();
  }

  // configure 'M00_AXIS' transactor
  if (xsc::utils::xsc_sim_manager::getInstanceParameterInt("axis_switch_0", "M00_AXIS_TLM_MODE") != 1)
  {
    mp_impl->M00_AXIS_INITIATOR_SOCKET->bind(*(mp_M00_AXIS_transactor->socket));
  
  }
  else
  {
    M00_AXIS_transactor_initiator_socket_stub = new xtlm::xtlm_axis_initiator_stub("socket",0);
    M00_AXIS_transactor_initiator_socket_stub->bind(*(mp_M00_AXIS_transactor->socket));
    mp_M00_AXIS_transactor->disable_transactor();
  }

  // configure 'S01_AXIS' transactor
  if (xsc::utils::xsc_sim_manager::getInstanceParameterInt("axis_switch_0", "S01_AXIS_TLM_MODE") != 1)
  {
    mp_impl->S01_AXIS_TARGET_SOCKET->bind(*(mp_S01_AXIS_transactor->socket));
  
  }
  else
  {
    S01_AXIS_transactor_target_socket_stub = new xtlm::xtlm_axis_target_stub("socket",0);
    S01_AXIS_transactor_target_socket_stub->bind(*(mp_S01_AXIS_transactor->socket));
    mp_S01_AXIS_transactor->disable_transactor();
  }

}

#endif // VCSSYSTEMC




#ifdef MTI_SYSTEMC
axis_switch_0::axis_switch_0(const sc_core::sc_module_name& nm) : axis_switch_0_sc(nm),  aclk("aclk"), aresetn("aresetn"), s_axis_tvalid("s_axis_tvalid"), s_axis_tready("s_axis_tready"), s_axis_tdata("s_axis_tdata"), s_axis_tlast("s_axis_tlast"), m_axis_tvalid("m_axis_tvalid"), m_axis_tready("m_axis_tready"), m_axis_tdata("m_axis_tdata"), m_axis_tlast("m_axis_tlast"), s_req_suppress("s_req_suppress"), s_decode_err("s_decode_err")
{
  // initialize pins
  mp_impl->aclk(aclk);
  mp_impl->aresetn(aresetn);
  mp_impl->s_req_suppress(s_req_suppress);
  mp_impl->s_decode_err(s_decode_err);

  // initialize transactors
  mp_S00_AXIS_transactor = NULL;
  mp_s_axis_tvalid_converter_0 = NULL;
  mp_s_axis_tready_converter_0 = NULL;
  mp_s_axis_tdata_converter_0 = NULL;
  mp_s_axis_tlast_converter_0 = NULL;
  mp_M00_AXIS_transactor = NULL;
  mp_m_axis_tvalid_converter = NULL;
  mp_m_axis_tready_converter = NULL;
  mp_m_axis_tlast_converter = NULL;
  mp_S01_AXIS_transactor = NULL;
  mp_s_axis_tvalid_converter_1 = NULL;
  mp_s_axis_tready_converter_1 = NULL;
  mp_s_axis_tdata_converter_1 = NULL;
  mp_s_axis_tlast_converter_1 = NULL;

  // initialize port junctures
  mp_s_axis_concat_tready = NULL;
  mp_s_axis_split_tdata = NULL;
  mp_s_axis_split_tlast = NULL;
  mp_s_axis_split_tvalid = NULL;

  // Instantiate Socket Stubs

  // configure S00_AXIS_transactor
    xsc::common_cpp::properties S00_AXIS_transactor_param_props;
    S00_AXIS_transactor_param_props.addLong("TDATA_NUM_BYTES", "1");
    S00_AXIS_transactor_param_props.addLong("TDEST_WIDTH", "0");
    S00_AXIS_transactor_param_props.addLong("TID_WIDTH", "0");
    S00_AXIS_transactor_param_props.addLong("TUSER_WIDTH", "0");
    S00_AXIS_transactor_param_props.addLong("HAS_TREADY", "1");
    S00_AXIS_transactor_param_props.addLong("HAS_TSTRB", "0");
    S00_AXIS_transactor_param_props.addLong("HAS_TKEEP", "0");
    S00_AXIS_transactor_param_props.addLong("HAS_TLAST", "1");
    S00_AXIS_transactor_param_props.addLong("FREQ_HZ", "100000000");
    S00_AXIS_transactor_param_props.addLong("HAS_RESET", "0");
    S00_AXIS_transactor_param_props.addFloat("PHASE", "0.0");
    S00_AXIS_transactor_param_props.addString("CLK_DOMAIN", "");
    S00_AXIS_transactor_param_props.addString("LAYERED_METADATA", "undef");
    S00_AXIS_transactor_param_props.addLong("TSIDE_BAND_1_WIDTH", "0");
    S00_AXIS_transactor_param_props.addLong("TSIDE_BAND_2_WIDTH", "0");

    mp_S00_AXIS_transactor = new xtlm::xaxis_pin2xtlm_t<2,2,2,2,1,1>("S00_AXIS_transactor", S00_AXIS_transactor_param_props);
  mp_s_axis_tvalid_converter_0 = new xsc::common::vectorN2scalar_converter<2>("s_axis_tvalid_converter_0");
  mp_s_axis_split_tvalid = new xsc::xsc_split<2, 2>("s_axis_split_tvalid");
  mp_s_axis_split_tvalid->in_port(s_axis_tvalid);
  mp_s_axis_split_tvalid->out_port[0](s_axis_split_tvalid_out_0);
    mp_s_axis_split_tvalid->add_mask(0,1,0);
  mp_s_axis_tvalid_converter_0->vector_in(s_axis_split_tvalid_out_0);
  mp_s_axis_tvalid_converter_0->scalar_out(m_s_axis_tvalid_converter_0_signal);
  mp_S00_AXIS_transactor->TVALID(m_s_axis_tvalid_converter_0_signal);
  mp_s_axis_tready_converter_0 = new xsc::common::scalar2vectorN_converter<2>("s_axis_tready_converter_0");
  mp_s_axis_concat_tready = new xsc::xsc_concatenator<2, 2>("s_axis_concat_tready");
  mp_s_axis_concat_tready->in_port[0](s_axis_concat_tready_out_0);
  mp_s_axis_concat_tready->out_port(s_axis_tready);
    mp_s_axis_concat_tready->offset_port(0, 0);
  mp_s_axis_tready_converter_0->scalar_in(m_s_axis_tready_converter_0_signal);
  mp_s_axis_tready_converter_0->vector_out(s_axis_concat_tready_out_0);
  mp_S00_AXIS_transactor->TREADY(m_s_axis_tready_converter_0_signal);
  mp_s_axis_tdata_converter_0 = new xsc::common::vector2vector_converter<16,16>("s_axis_tdata_converter_0");
  mp_s_axis_split_tdata = new xsc::xsc_split<16, 2>("s_axis_split_tdata");
  mp_s_axis_split_tdata->in_port(s_axis_tdata);
  mp_s_axis_split_tdata->out_port[0](s_axis_split_tdata_out_0);
    mp_s_axis_split_tdata->add_mask(0,8,0);
  mp_s_axis_tdata_converter_0->vector_in(s_axis_split_tdata_out_0);
  mp_s_axis_tdata_converter_0->vector_out(m_s_axis_tdata_converter_0_signal);
  mp_S00_AXIS_transactor->TDATA(m_s_axis_tdata_converter_0_signal);
  mp_s_axis_tlast_converter_0 = new xsc::common::vectorN2scalar_converter<2>("s_axis_tlast_converter_0");
  mp_s_axis_split_tlast = new xsc::xsc_split<2, 2>("s_axis_split_tlast");
  mp_s_axis_split_tlast->in_port(s_axis_tlast);
  mp_s_axis_split_tlast->out_port[0](s_axis_split_tlast_out_0);
    mp_s_axis_split_tlast->add_mask(0,1,0);
  mp_s_axis_tlast_converter_0->vector_in(s_axis_split_tlast_out_0);
  mp_s_axis_tlast_converter_0->scalar_out(m_s_axis_tlast_converter_0_signal);
  mp_S00_AXIS_transactor->TLAST(m_s_axis_tlast_converter_0_signal);
  mp_S00_AXIS_transactor->CLK(aclk);
  m_S00_AXIS_transactor_rst_signal.write(1);
  mp_S00_AXIS_transactor->RST(m_S00_AXIS_transactor_rst_signal);
  // configure M00_AXIS_transactor
    xsc::common_cpp::properties M00_AXIS_transactor_param_props;
    M00_AXIS_transactor_param_props.addLong("TDATA_NUM_BYTES", "1");
    M00_AXIS_transactor_param_props.addLong("TDEST_WIDTH", "0");
    M00_AXIS_transactor_param_props.addLong("TID_WIDTH", "0");
    M00_AXIS_transactor_param_props.addLong("TUSER_WIDTH", "0");
    M00_AXIS_transactor_param_props.addLong("HAS_TREADY", "1");
    M00_AXIS_transactor_param_props.addLong("HAS_TSTRB", "0");
    M00_AXIS_transactor_param_props.addLong("HAS_TKEEP", "0");
    M00_AXIS_transactor_param_props.addLong("HAS_TLAST", "1");
    M00_AXIS_transactor_param_props.addLong("FREQ_HZ", "100000000");
    M00_AXIS_transactor_param_props.addLong("HAS_RESET", "0");
    M00_AXIS_transactor_param_props.addFloat("PHASE", "0.0");
    M00_AXIS_transactor_param_props.addString("CLK_DOMAIN", "");
    M00_AXIS_transactor_param_props.addString("LAYERED_METADATA", "undef");
    M00_AXIS_transactor_param_props.addLong("TSIDE_BAND_1_WIDTH", "0");
    M00_AXIS_transactor_param_props.addLong("TSIDE_BAND_2_WIDTH", "0");

    mp_M00_AXIS_transactor = new xtlm::xaxis_xtlm2pin_t<1,1,1,1,1,1>("M00_AXIS_transactor", M00_AXIS_transactor_param_props);
  mp_m_axis_tvalid_converter = new xsc::common::scalar2vectorN_converter<1>("m_axis_tvalid_converter");
  mp_m_axis_tvalid_converter->scalar_in(m_m_axis_tvalid_converter_signal);
  mp_m_axis_tvalid_converter->vector_out(m_axis_tvalid);
  mp_M00_AXIS_transactor->TVALID(m_m_axis_tvalid_converter_signal);
  mp_m_axis_tready_converter = new xsc::common::vectorN2scalar_converter<1>("m_axis_tready_converter");
  mp_m_axis_tready_converter->vector_in(m_axis_tready);
  mp_m_axis_tready_converter->scalar_out(m_m_axis_tready_converter_signal);
  mp_M00_AXIS_transactor->TREADY(m_m_axis_tready_converter_signal);
  mp_M00_AXIS_transactor->TDATA(m_axis_tdata);
  mp_m_axis_tlast_converter = new xsc::common::scalar2vectorN_converter<1>("m_axis_tlast_converter");
  mp_m_axis_tlast_converter->scalar_in(m_m_axis_tlast_converter_signal);
  mp_m_axis_tlast_converter->vector_out(m_axis_tlast);
  mp_M00_AXIS_transactor->TLAST(m_m_axis_tlast_converter_signal);
  mp_M00_AXIS_transactor->CLK(aclk);
  m_M00_AXIS_transactor_rst_signal.write(1);
  mp_M00_AXIS_transactor->RST(m_M00_AXIS_transactor_rst_signal);
  // configure S01_AXIS_transactor
    xsc::common_cpp::properties S01_AXIS_transactor_param_props;
    S01_AXIS_transactor_param_props.addLong("TDATA_NUM_BYTES", "1");
    S01_AXIS_transactor_param_props.addLong("TDEST_WIDTH", "0");
    S01_AXIS_transactor_param_props.addLong("TID_WIDTH", "0");
    S01_AXIS_transactor_param_props.addLong("TUSER_WIDTH", "0");
    S01_AXIS_transactor_param_props.addLong("HAS_TREADY", "1");
    S01_AXIS_transactor_param_props.addLong("HAS_TSTRB", "0");
    S01_AXIS_transactor_param_props.addLong("HAS_TKEEP", "0");
    S01_AXIS_transactor_param_props.addLong("HAS_TLAST", "1");
    S01_AXIS_transactor_param_props.addLong("FREQ_HZ", "100000000");
    S01_AXIS_transactor_param_props.addLong("HAS_RESET", "0");
    S01_AXIS_transactor_param_props.addFloat("PHASE", "0.0");
    S01_AXIS_transactor_param_props.addString("CLK_DOMAIN", "");
    S01_AXIS_transactor_param_props.addString("LAYERED_METADATA", "undef");
    S01_AXIS_transactor_param_props.addLong("TSIDE_BAND_1_WIDTH", "0");
    S01_AXIS_transactor_param_props.addLong("TSIDE_BAND_2_WIDTH", "0");

    mp_S01_AXIS_transactor = new xtlm::xaxis_pin2xtlm_t<2,2,2,2,1,1>("S01_AXIS_transactor", S01_AXIS_transactor_param_props);
  mp_s_axis_tvalid_converter_1 = new xsc::common::vectorN2scalar_converter<2>("s_axis_tvalid_converter_1");
  
  mp_s_axis_split_tvalid->out_port[1](s_axis_split_tvalid_out_1);
    mp_s_axis_split_tvalid->add_mask(1,2,1);
  mp_s_axis_tvalid_converter_1->vector_in(s_axis_split_tvalid_out_1);
  mp_s_axis_tvalid_converter_1->scalar_out(m_s_axis_tvalid_converter_1_signal);
  mp_S01_AXIS_transactor->TVALID(m_s_axis_tvalid_converter_1_signal);
  mp_s_axis_tready_converter_1 = new xsc::common::scalar2vectorN_converter<2>("s_axis_tready_converter_1");
  mp_s_axis_concat_tready->in_port[1](s_axis_concat_tready_out_1);
  mp_s_axis_concat_tready->offset_port(1, 1);
  mp_s_axis_tready_converter_1->scalar_in(m_s_axis_tready_converter_1_signal);
  mp_s_axis_tready_converter_1->vector_out(s_axis_concat_tready_out_1);
  mp_S01_AXIS_transactor->TREADY(m_s_axis_tready_converter_1_signal);
  mp_s_axis_tdata_converter_1 = new xsc::common::vector2vector_converter<16,16>("s_axis_tdata_converter_1");
  
  mp_s_axis_split_tdata->out_port[1](s_axis_split_tdata_out_1);
    mp_s_axis_split_tdata->add_mask(1,16,8);
  mp_s_axis_tdata_converter_1->vector_in(s_axis_split_tdata_out_1);
  mp_s_axis_tdata_converter_1->vector_out(m_s_axis_tdata_converter_1_signal);
  mp_S01_AXIS_transactor->TDATA(m_s_axis_tdata_converter_1_signal);
  mp_s_axis_tlast_converter_1 = new xsc::common::vectorN2scalar_converter<2>("s_axis_tlast_converter_1");
  
  mp_s_axis_split_tlast->out_port[1](s_axis_split_tlast_out_1);
    mp_s_axis_split_tlast->add_mask(1,2,1);
  mp_s_axis_tlast_converter_1->vector_in(s_axis_split_tlast_out_1);
  mp_s_axis_tlast_converter_1->scalar_out(m_s_axis_tlast_converter_1_signal);
  mp_S01_AXIS_transactor->TLAST(m_s_axis_tlast_converter_1_signal);
  mp_S01_AXIS_transactor->CLK(aclk);
  m_S01_AXIS_transactor_rst_signal.write(1);
  mp_S01_AXIS_transactor->RST(m_S01_AXIS_transactor_rst_signal);

  // initialize transactors stubs
  S00_AXIS_transactor_target_socket_stub = nullptr;
  M00_AXIS_transactor_initiator_socket_stub = nullptr;
  S01_AXIS_transactor_target_socket_stub = nullptr;

}

void axis_switch_0::before_end_of_elaboration()
{
  // configure 'S00_AXIS' transactor
  if (xsc::utils::xsc_sim_manager::getInstanceParameterInt("axis_switch_0", "S00_AXIS_TLM_MODE") != 1)
  {
    mp_impl->S00_AXIS_TARGET_SOCKET->bind(*(mp_S00_AXIS_transactor->socket));
  
  }
  else
  {
    S00_AXIS_transactor_target_socket_stub = new xtlm::xtlm_axis_target_stub("socket",0);
    S00_AXIS_transactor_target_socket_stub->bind(*(mp_S00_AXIS_transactor->socket));
    mp_S00_AXIS_transactor->disable_transactor();
  }

  // configure 'M00_AXIS' transactor
  if (xsc::utils::xsc_sim_manager::getInstanceParameterInt("axis_switch_0", "M00_AXIS_TLM_MODE") != 1)
  {
    mp_impl->M00_AXIS_INITIATOR_SOCKET->bind(*(mp_M00_AXIS_transactor->socket));
  
  }
  else
  {
    M00_AXIS_transactor_initiator_socket_stub = new xtlm::xtlm_axis_initiator_stub("socket",0);
    M00_AXIS_transactor_initiator_socket_stub->bind(*(mp_M00_AXIS_transactor->socket));
    mp_M00_AXIS_transactor->disable_transactor();
  }

  // configure 'S01_AXIS' transactor
  if (xsc::utils::xsc_sim_manager::getInstanceParameterInt("axis_switch_0", "S01_AXIS_TLM_MODE") != 1)
  {
    mp_impl->S01_AXIS_TARGET_SOCKET->bind(*(mp_S01_AXIS_transactor->socket));
  
  }
  else
  {
    S01_AXIS_transactor_target_socket_stub = new xtlm::xtlm_axis_target_stub("socket",0);
    S01_AXIS_transactor_target_socket_stub->bind(*(mp_S01_AXIS_transactor->socket));
    mp_S01_AXIS_transactor->disable_transactor();
  }

}

#endif // MTI_SYSTEMC




axis_switch_0::~axis_switch_0()
{
  delete mp_S00_AXIS_transactor;
  delete mp_s_axis_tvalid_converter_0;
  delete mp_s_axis_tready_converter_0;
  delete mp_s_axis_tdata_converter_0;
  delete mp_s_axis_tlast_converter_0;

  delete mp_M00_AXIS_transactor;
  delete mp_m_axis_tvalid_converter;
  delete mp_m_axis_tready_converter;
  delete mp_m_axis_tlast_converter;

  delete mp_S01_AXIS_transactor;
  delete mp_s_axis_tvalid_converter_1;
  delete mp_s_axis_tready_converter_1;
  delete mp_s_axis_tdata_converter_1;
  delete mp_s_axis_tlast_converter_1;

  delete mp_s_axis_concat_tready;
  delete mp_s_axis_split_tdata;
  delete mp_s_axis_split_tlast;
  delete mp_s_axis_split_tvalid;
}

#ifdef MTI_SYSTEMC
SC_MODULE_EXPORT(axis_switch_0);
#endif

#ifdef XM_SYSTEMC
XMSC_MODULE_EXPORT(axis_switch_0);
#endif

#ifdef RIVIERA
SC_MODULE_EXPORT(axis_switch_0);
#endif

