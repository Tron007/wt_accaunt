/*
 * wtaccounts.h

 *
 *  Created on: Nov 7, 2016
 *      Author: judge
 */


#ifndef WTACCOUNTS_H
#define WTACCOUNTS_H

#include <Wt/WApplication>

#include "ui_wtaccounts.h"






class ReportResource2
{	std::string gow="";
public:

	ReportResource2(std::string d) : gow(d)
    {}
std::string ret(){return gow;}
};

class WtAccounts : public Wt::WApplication
{

public:
	WtAccounts(const Wt::WEnvironment& env);
	~WtAccounts();

	// declare slots here
	void data_load_user_account_tab();
	void create_user_tab_save_data();
	void edit_subscriber_load_data();
	// ***


    void p_account_operation_CHECK(std::string operation_name);
    //data
    void p_account_operation_Data(std::string operation_name);
    //report
    void p_account_operation_Report(std::string operation_name);
    void p_account_operation_create_Report(std::string operation_name);
    void Configure_edited_report_data_Report(std::string operation_name);
    //Serarch
    void Search_tree_Names(std::string operation_name);


	void subscriber_show_operation_tab(std::string operation_name);
	void subscriber_name_dialog(std::string operation_name);
	void subscriber_group_dialog(std::string operation_name);

	void subscriber_phone_number_dialog(std::string operation_name);
	void create_phone_number_dialog(std::string operation_name);
	void delete_phone_number_dialog(std::string operation_name);

	void subscriber_ip_address_dialog(std::string operation_name);
	void create_ip_address_dialog(std::string operation_name);
	void delete_ip_address_dialog(std::string operation_name);

	void subscriber_service_dialog(std::string operation_name);
	void add_service_dialog(std::string operation_name);
	void delete_service_dialog(std::string operation_name);

	void subscriber_tariff_plan_dialog(std::string operation_name);
	void subscriber_contact_dialog(std::string operation_name);
	void save_data_and_close_tab(std::string operation_name);

	void manage_new_call_tariff_dialog();

	// administrative functions
	// *************************************************************************
	void manage_services_dialog();
	void create_new_service_dialog();
	// *************************************************************************

	void subscriber_fullName_changed();

	void change_to_edit_new_call_tariff_dialog();

private:
	Ui_WtAccounts *ui;

};






#endif // WTACCOUNTS_H
