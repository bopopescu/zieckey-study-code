#include "stdafx.h"

#include "../include/user_center.h"
#include "logic/include/run_config.h"
#include "json/json.h"

namespace nm
{
	//     class LoginListener : public HttpRequest::Listener
	//     {
	//     public:
	//         LoginListener()
	//         {
	// 
	//         }
	// 
	//         //! \brief <b>Summary:</b>
	//         //! 	This function is called by HTTPWork(observable) to notify
	//         //! Listeners that this HTTP work is done, and the received data 
	//         //! from server is preserved in m_ptrRecvMDStream
	//         //! \note This function is called in another thread, you <b>MUST</b>
	//         //!     be care about the multi thread-safe problem
	//         //! \param  pw, the HTTPWork which is listened by this listener.
	//         //! you can call HTTPWork::getRecvDataStream() to get the server response string.
	//         virtual void OnFinishOKT( HttpRequest* pw )
	//         {
	// 
	//         }
	// 
	//         //! \brief <b>Summary:</b>
	//         //! 	This function is called by HTTPWork(observable) to notify
	//         //! Listeners that the work is no done because something error.
	//         //! \note This function is called in another thread, you <b>MUST</b>
	//         //!     be care about the multi thread-safe problem
	//         //! \param hec, error code
	//         //! \param pw, the HTTPWork which is listened by this listener
	//         virtual void OnFinishErrorT(HttpRequest::HttpErrorCode hec, HttpRequest* pw )
	//         {
	// 
	//         }
	//     };
#define  H_MBS_TO_UTF8(x) osl::StringUtil::mbsToUtf8(x)

	UserCenter::UserCenter( net::CURLService* service ) : curl_service_(service)
	{

	}

	UserCenter::~UserCenter()
	{
		curl_service_ = NULL;
	}

	bool UserCenter::DeleteProducedInformation( const std::string& guid, net::CURLWork::Listener* listener )
	{
		std::stringstream oss;
		oss << '"' << "guid=" << guid << '"';
		return DoHttpPost(s_pRunConfig->GetLoginURL(), oss.str(), listener);
	}

	bool UserCenter::SelectConsumeByGoodsGUID( const std::string& goodsGuid, const std::string& begintime, const std::string& endtime, const std::string& firstRowNumber, const std::string& lastRowNumber, const std::string& sortField,/*the original value, no base64 */ const std::string& sortType , net::CURLWork::Listener* listener )
	{
		//             mapvl.insert(make_pair("goodsGuid",CStringA(GoodsGuid)));
		//             mapvl.insert(make_pair("begintime",CStringA(begintime)));
		//             mapvl.insert(make_pair("endtime",CStringA(endtime)));
		// 
		//             mapvl.insert(make_pair("firstRowNumber",firstRowNumber));
		//             mapvl.insert(make_pair("lastRowNumber",lastRowNumber));
		//             string sortValue=  CStringA(sortField);
		//             mapvl.insert(make_pair("sortField",CommonClass::Base64Encode(sortValue)));
		//             mapvl.insert(make_pair("sortType",CStringA(sortType)));

		std::ostringstream oss;
		oss << '"';
		oss << "goodsGuid=" << goodsGuid 
			<< "&begintime=" << begintime
			<< "&endtime=" << endtime
			<< "&firstRowNumber=" << firstRowNumber
			<< "&lastRowNumber=" << lastRowNumber
			<< "&sortField=" << osl::Base64::encode(sortField.c_str(), sortField.length())
			<< "&sortType=" << sortType
			<< '"';
		return DoHttpPost(s_pRunConfig->GetSelectConsumeByGoodsGUIDURL(), oss.str(), listener);
	}

	bool UserCenter::Login(const std::string& username, const std::string& pwd, net::CURLWork::Listener* listener)
	{
		std::stringstream oss;
		oss << '"' << "loginName=" << username << "&loginPwd=" << pwd << '"';
		return DoHttpPost(s_pRunConfig->GetDeleteProducedInformationURL(), oss.str(), listener);
	}

	bool UserCenter::ParseRespMsg( const char* login_resp_msg, size_t login_resp_msg_len, model::ListUMSBaseUserinfo& user_info_list )
	{
		json::JSONArray ja;
		if (ja.parse(login_resp_msg, login_resp_msg_len) == 0)
		{
			return false;
		}

		json::JSONArray::Iterator it(ja.begin());
		json::JSONArray::Iterator ite(ja.end());
		for (; it != ite; ++it)
		{
			if (!it->getPointer()->getType() == json::OT_OBJECT)
			{
				continue;
			}
			json::JSONObject* jo = static_cast<json::JSONObject*>(it->getPointer());
			if (!jo)
			{
				continue;
			}
			user_info_list.push_back(model::UMSBaseUserinfo());
			model::UMSBaseUserinfo& user_info = *user_info_list.rbegin();
			user_info.user_guid          = jo->getString(H_MBS_TO_UTF8("GUID"));
			user_info.user_type          = jo->getInteger(H_MBS_TO_UTF8("用户类型"));
			user_info.branch_guid        = jo->getString(H_MBS_TO_UTF8("商户GUID"));
			user_info.user_name          = jo->getString(H_MBS_TO_UTF8("用户姓名"));
			user_info.user_num           = jo->getString(H_MBS_TO_UTF8("工号"));
			user_info.user_id_num        = jo->getString(H_MBS_TO_UTF8("身份证号码"));
			user_info.user_postion       = jo->getString(H_MBS_TO_UTF8("职务"));
			user_info.user_postion_guid  = jo->getString(H_MBS_TO_UTF8("岗位GUID"));
			user_info.user_sex           = jo->getInteger(H_MBS_TO_UTF8("性别"));
			user_info.user_phone_num     = jo->getString(H_MBS_TO_UTF8("电话"));
			user_info.user_login_name    = jo->getString(H_MBS_TO_UTF8("登录名称"));
			user_info.user_pwd           = jo->getString(H_MBS_TO_UTF8("密码"));
			user_info.user_dept_guid     = jo->getString(H_MBS_TO_UTF8("部门GUID"));
			user_info.branch_hotline     = jo->getString(H_MBS_TO_UTF8("热线电话"));
			user_info.branch_address     = jo->getString(H_MBS_TO_UTF8("地址"));
			user_info.branch_name        = jo->getString(H_MBS_TO_UTF8("门店名称"));
			user_info.user_power         = jo->getString(H_MBS_TO_UTF8("权限"));
			user_info.user_role          = jo->getString(H_MBS_TO_UTF8("角色"));
			user_info.branch_type        = jo->getInteger(H_MBS_TO_UTF8("商户类型"));
			user_info.branch_city_id     = jo->getInteger(H_MBS_TO_UTF8("所在城市ID"));
			user_info.branch_city_name   = jo->getString(H_MBS_TO_UTF8("所在城市ID"));
			user_info.user_dept_name     = jo->getString(H_MBS_TO_UTF8("部门名称"));
			user_info.branch_group_guid  = jo->getString(H_MBS_TO_UTF8("集团GUID"));
			user_info.branch_group_name  = jo->getString(H_MBS_TO_UTF8("集团名称"));
			user_info.opt_time           = jo->getInteger(H_MBS_TO_UTF8("创建时间"));
		}

		return true;
	}


	bool UserCenter::ParseRespMsg(const char* resp_msg, size_t resp_msg_len, model::ListViewOPRProductConsumDetail& model_list)
	{
		json::JSONArray ja;
		if (ja.parse(resp_msg, resp_msg_len) == 0)
		{
			return false;
		}
		json::JSONArray::Iterator it(ja.begin());
		json::JSONArray::Iterator ite(ja.end());
		for (; it != ite; ++it)
		{
			if (!it->getPointer()->getType() == json::OT_OBJECT)
			{
				continue;
			}
			json::JSONObject* jo = static_cast<json::JSONObject*>(it->getPointer());
			if (!jo)
			{
				continue;
			}
			model_list.push_back(model::ViewOPRProductConsumDetail());
			model::ViewOPRProductConsumDetail& model_info = *model_list.rbegin();
			model_info.consumetype=jo->getString(H_MBS_TO_UTF8(""));
			model_info.opertime=jo->getInteger(H_MBS_TO_UTF8(""));
			model_info.quantity=jo->getDouble(H_MBS_TO_UTF8(""));
			model_info.unitprice=jo->getDouble(H_MBS_TO_UTF8(""));
		}
		return true;
	}

	bool UserCenter::ParseRespMsg( const char* resp_msg, size_t resp_msg_len, model::ViewOPRProductConsumDetailPagging& page )
	{
		if (resp_msg_len == 0)
		{
			return false;
		}

		Json::Reader m_read;
		Json::Value m_vl;
		Json::Value m_van;
		if (!m_read.parse(resp_msg, resp_msg + resp_msg_len, m_vl))
		{
			return false;
		}

		page.page_count = m_vl[H_MBS_TO_UTF8("总行数")].asInt();
		m_van = m_vl[H_MBS_TO_UTF8("VIEW_OPR_货品消费明细")];

		for (unsigned int i=0;i < m_van.size();i++)
		{
			page.view_opr_pruduct_consum_detail.push_back(model::ViewOPRProductConsumDetail());
			model::ViewOPRProductConsumDetail& detail = *page.view_opr_pruduct_consum_detail.rbegin();
			detail.consumetype = m_van[i]["F_ConsumeType"].asString();
			detail.opertime    = m_van[i]["F_OperTime"].asString();
			detail.quantity    = m_van[i]["F_Quantity"].asDouble();
			detail.subtotal    = m_van[i]["F_Subtotal"].asDouble();
			detail.unitprice   = m_van[i]["F_UnitPrice"].asDouble();
		}

		return true;
	}

	bool UserCenter::ParseRespMsg(const char* resp_msg, size_t resp_msg_len, model::ViewMember& member)
	{
		if (resp_msg_len == 0)
		{
			return false;
		}

		Json::Reader m_read;
		Json::Value vl;
		Json::Value m_van;
		if (!m_read.parse(resp_msg, resp_msg + resp_msg_len, vl))
		{
			return false;
		}

		member.member_name=vl[H_MBS_TO_UTF8("会员名称")].asString();
		member.member_guid=vl["GUID"].asString();
		member.member_cardnum=vl[H_MBS_TO_UTF8("卡号")].asString();
		member.member_id_num=vl[H_MBS_TO_UTF8("证件号码")].asString();
		member.member_phone=vl[H_MBS_TO_UTF8("手机")].asString();
		member.member_card_balance=vl[H_MBS_TO_UTF8("卡余额")].asDouble();
		member.member_level_guid=vl[H_MBS_TO_UTF8("会员级别")].asString();
		member.member_type=vl[H_MBS_TO_UTF8("会员类型")].asInt();

		return true;
	}
	bool UserCenter::ParseRespMsg( const char* resp_msg, size_t resp_msg_len, model::uniteMemberConsumeInfoPagging& model_list )
	{
		if (resp_msg_len == 0)
		{
			return false;
		}
		Json::Reader m_read;
		Json::Value vl;
		Json::Value rst;
		if (!m_read.parse(resp_msg, resp_msg + resp_msg_len, vl))
		{
			return false;
		}
		model_list.page_count=vl[H_MBS_TO_UTF8("总行数")].asInt();
		rst=vl[H_MBS_TO_UTF8("ums_门店联乐会员消费信息")];
		for (unsigned int i=0;i < rst.size();i++)
		{
			model_list.list_UniteMemberConsumeInfo.push_back(model::uniteMemberConsumeInfo());
			nm::model::uniteMemberConsumeInfo& temp=*model_list.list_UniteMemberConsumeInfo.rbegin();			
			temp.operator_time=rst[i][H_MBS_TO_UTF8("操作时间")].asString();  //操作时间			
			temp.money_need=rst[i][H_MBS_TO_UTF8("应付金额")].asDouble();
			temp.money_forfree=rst[i][H_MBS_TO_UTF8("优惠金额")].asDouble();
			temp.money_pay=rst[i][H_MBS_TO_UTF8("实收金额")].asDouble();
			temp.balance=rst[i][H_MBS_TO_UTF8("余额")].asDouble();
			temp.billno=rst[i][H_MBS_TO_UTF8("单号")].asString();
			temp.isExistProduct=rst[i][H_MBS_TO_UTF8("是否有产品")].asInt();
			temp.member_guid=rst[i][H_MBS_TO_UTF8("会员GUID")].asString();
			temp.accountNo=rst[i][H_MBS_TO_UTF8("帐户号")].asString();
		}	

        return true;
	}

	bool UserCenter::ParseRespMsg( const char* resp_msg, size_t resp_msg_len, model::UMSUniteMemberInfo& model_list )
	{
		if(resp_msg_len<1) return false;		
		Json::Reader dr;
		Json::Value vl;		
		if(dr.parse(resp_msg,vl))	
		{
			model_list.unite_member_name=vl[H_MBS_TO_UTF8("会员名称")].asString();
			model_list.unite_card_no=vl[H_MBS_TO_UTF8("卡号")].asString();
			model_list.unite_member_phone=vl[H_MBS_TO_UTF8("手机")].asString();
			model_list.unite_total_cash=vl[H_MBS_TO_UTF8("累计消费金额")].asDouble();
			model_list.unite_acount_no=vl[H_MBS_TO_UTF8("账户号")].asString();
		}
		return true;
	}


	bool UserCenter::ParseRespMsg( const char* resp_msg, size_t resp_msg_len, model::ListVIEW_Company_Coupon& model_list )
	{
		if (resp_msg_len == 0)
		{
			return false;
		}

		Json::Reader reader;
		Json::Value root_value;
		if (!reader.parse(resp_msg, resp_msg + resp_msg_len, root_value))
		{
			return false;
		}

		for (unsigned int i=0; i < root_value.size(); i++)
		{
			model_list.push_back(model::VIEW_Company_Coupon());
			model::VIEW_Company_Coupon& d = *model_list.rbegin();
			d.coupon_guid=root_value[i]["GUID"].asString();//必须
			d.coupon_name=root_value[i][H_MBS_TO_UTF8("优惠券名称")].asString();//必须
			d.instructions=root_value[i][H_MBS_TO_UTF8("使用说明")].asString();//必须
			d.discountmoney = root_value[i][H_MBS_TO_UTF8("优惠后金额")].asDouble();//必须
			//             d.logo=root_value[i][H_MBS_TO_UTF8("标识")].asInt();
			//             d.note=root_value[i][H_MBS_TO_UTF8("备注")].asString();
			//             //tt.audit=root_value[i][H_MBS_TO_UTF8("审核")].asInt();
			//             d.coupon_id=root_value[i][H_MBS_TO_UTF8("优惠券ID")].asInt();
			//             d.condition2=root_value[i][H_MBS_TO_UTF8("条件2")].asDouble();
			//             d.merchant_guid=root_value[i][H_MBS_TO_UTF8("商户GUID")].asString();
			//             d.coupon_name_type=root_value[i][H_MBS_TO_UTF8("优惠券名称种类")].asString();
			//             d.startdate=root_value[i][H_MBS_TO_UTF8("开始日期")].asString();
			//             d.starttime=root_value[i][H_MBS_TO_UTF8("起始时段")].asString();
			//             d.enddate=root_value[i][H_MBS_TO_UTF8("截止日期")].asString();
			//             d.endtime=root_value[i][H_MBS_TO_UTF8("截至时段")].asString();
			//             d.useweek=root_value[i][H_MBS_TO_UTF8("使用星期")].asString();
			//             d.member_lever=root_value[i][H_MBS_TO_UTF8("会员级别")].asString();
			//             d.formula=root_value[i][H_MBS_TO_UTF8("公式")].asString();
		}
		return true;
	}

	//bool UserCenter::ParseRespMsg(const char* resp_msg, size_t resp_msg_len, model::ViewOPRProductConsumDetailPagging& page)
	//{
	//	json::JSONObject ja;
	//	if (ja.parse(resp_msg, resp_msg_len) == 0)
	//	{
	//		return false;
	//	}
	//	json::JSONArray::Iterator it(ja.begin());
	//	json::JSONArray::Iterator ite(ja.end());
	//	for (; it != ite; ++it)
	//	{
	//		if (!it->getPointer()->getType() == json::OT_OBJECT)
	//		{
	//			continue;
	//		}
	//		json::JSONObject* jo = static_cast<json::JSONObject*>(it->getPointer());
	//		if (!jo)
	//		{
	//			continue;
	//		}
	//		page.push_back(model::ViewOPRProductConsumDetailPagging());
	//		model::ViewOPRProductConsumDetailPagging& model_info = *page.rbegin();
	//		model_info.page_count=jo->getString(H_MBS_TO_UTF8(""));
	//	}
	//	return true;
	//}

	//bool UserCenter::ParseRespMsg(const char* resp_msg, size_t resp_msg_len, model::ListUMSOPRMarkShopLevel& page)
	//{
	//	json::JSONArray ja;
	//	if (ja.parse(resp_msg, resp_msg_len) == 0)
	//	{
	//		return false;
	//	}
	//	json::JSONArray::Iterator it(ja.begin());
	//	json::JSONArray::Iterator ite(ja.end());
	//	for (; it != ite; ++it)
	//	{
	//		if (!it->getPointer()->getType() == json::OT_OBJECT)
	//		{
	//			continue;
	//		}
	//		json::JSONObject* jo = static_cast<json::JSONObject*>(it->getPointer());
	//		if (!jo)
	//		{
	//			continue;
	//		}
	//		page.push_back(model::UMSOPRMarkShopLevel());
	//		model::UMSOPRMarkShopLevel& model_info = *page.rbegin();
	//		model_info.create_time=jo->getInteger(H_MBS_TO_UTF8(""));
	//		model_info.creator=jo->getString(H_MBS_TO_UTF8(""));
	//		model_info.guid=jo->getString(H_MBS_TO_UTF8(""));
	//		model_info.id=jo->getInteger(H_MBS_TO_UTF8(""));
	//		model_info.market_level=jo->getString(H_MBS_TO_UTF8(""));
	//		model_info.note=jo->getString(H_MBS_TO_UTF8(""));
	//		model_info.shop_id=jo->getInteger(H_MBS_TO_UTF8(""));
	//		model_info.tag=jo->getInteger(H_MBS_TO_UTF8(""));
	//		model_info.update_time=jo->getInteger(H_MBS_TO_UTF8(""));
	//		model_info.updater=jo->getString(H_MBS_TO_UTF8(""));
	//	}
	//	return true;
	//}

	bool UserCenter::DoHttpPost( const std::string& url, const std::string& input_data, net::CURLWork::Listener* listener )
	{
		//         nm::HttpPost post(url.c_str(), 10, false);
		//         post.AddParameter("loginName", username.c_str());
		//         post.AddParameter("loginPwd", pwd.c_str());
		//         post.DoRequest();

		//oss << "ApiKey=" << string(CStringA(CommonClass::loginname)) << "&Sign=" << string(CStringA(CommonClass::pwd)) << "&";
		std::string data;
		data.reserve(input_data.length() + 128);
		data = input_data;
		if (data.length() > 2)
		{
			data[data.length() - 1] = '&';
		}
		else
		{
			data.resize(data.length() - 1);
		}
		data += "ApiKey=";
		data += user_name_;
		data += "&Sign=";
		data += user_pwd_;
		data += "\"";

		net::HttpPostWorkPtr work = new net::HttpPostWork(url);
		work->addListener(listener);
		//work->setRawHttpBodyContent(osl::StringUtil::mbsToUtf8(oss.str()));
		return curl_service_->addWorkT(work);
		//         work->setBlockingDoHttpRequest(true);
		//         return work->doHttpBlockingRequest();
	}

	bool UserCenter::DoHttpPost( const std::string& url, const stringstringmap& kvmap, net::CURLWork::Listener* listener )
	{
		std::ostringstream oss;
		oss << '"';
		stringstringmap::const_iterator it = kvmap.begin();
		for(; it!=kvmap.end(); ++it)
		{
			oss << it->first << "=" << it->second << '&';
		}
		std::string str = oss.str();
		str[str.length() - 1] = '"';
		return DoHttpPost(url, str, listener);
	}

	void UserCenter::SetUserNamePwd( const std::string& user_name, const std::string& pwd )
	{
		user_name_ = user_name;
		user_pwd_  = pwd;
	}

	bool UserCenter::SelectMemberInfoByDynamicCode( const std::string& dynamicCode, net::CURLWork::Listener* listener )
	{
		//maptest.insert(make_pair("dynamicCode", CStringA(code))); //
		std::stringstream body_data;
		body_data << '"' 
			<< "dynamicCode=" << dynamicCode 
			<< '"';
		return DoHttpPost(s_pRunConfig->GetSelectMemberInfoByDynamicCodeURL(), body_data.str(), listener);
	}

	bool UserCenter::SelectStoreTicketInfoList( const std::string& money, const std::string& shopGuid, const std::string& memberlevelGuid, const int flag, net::CURLWork::Listener* listener )
	{
		//         map<string,string> maptest;
		//         maptest.insert(make_pair( "money",paymoney));
		//         maptest.insert(make_pair( "shopGuid",CStringA(CommonClass::shopguid)));
		//         maptest.insert(make_pair( "memberlevelGuid", Cmembercash.CmemberLevel)); 
		//         if (Cmembercash.CmemberType==1||Cmembercash.CmemberType==3)
		//         {
		//             maptest.insert(make_pair( "flag","1")); 
		//         }
		//         else
		//         {
		//             maptest.insert(make_pair( "flag","2")); 
		//         }

		std::stringstream body_data;
		body_data << '"' 
			<< "money=" << money 
			<< "&shopGuid=" << shopGuid
			<< "&memberlevelGuid=" << memberlevelGuid
			<< "&flag=" << flag
			<< '"';
		return DoHttpPost(s_pRunConfig->GetSelectStoreTicketInfoListURL(), body_data.str(), listener);
	}

	bool UserCenter::GetCardPwdByCardID( const std::string& shopGuid, const std::string& physicalCard, net::CURLWork::Listener* listener )
	{
		//             maptest.insert(make_pair("shopGuid", CStringA(CommonClass::shopguid))); 
		//             maptest.insert(make_pair("physicalCard", CStringA(cardNo))); //¿¨ºÅ

		std::stringstream body_data;
		body_data << '"' 
			<< "shopGuid=" << shopGuid
			<< "&physicalCard=" << physicalCard
			<< '"';
		return DoHttpPost(s_pRunConfig->GetGetCardPwdByCardIDURL(), body_data.str(), listener);
	}

	bool UserCenter::SelectMemberInfoByCardID( const std::string& shopGuid, const std::string& memberGUID, net::CURLWork::Listener* listener )
	{
		//             maptest.insert(make_pair("shopGuid", CStringA(CommonClass::shopguid))); 
		//             maptest.insert(make_pair("memberGUID", CStringA(m_huiyuangid))); //会员GUID
		std::stringstream body_data;
		body_data << '"' 
			<< "shopGuid=" << shopGuid
			<< "&memberGUID=" << memberGUID
			<< '"';
		return DoHttpPost(s_pRunConfig->GetSelectMemberInfoByCardIDURL(), body_data.str(), listener);
	}

	bool UserCenter::InsertCardInfo( const std::string& shopGuid, const std::string& cardID, const std::string& physicalCard, const std::string& operatorName, net::CURLWork::Listener* listener )
	{
		//             map<string,string>  mapValue;
		//             mapValue.insert(make_pair("shopGuid",CStringA(CommonClass::shopguid)));
		//             mapValue.insert(make_pair("cardID",CStringA(m_cardno))); 
		//             mapValue.insert(make_pair("physicalCard",CStringA(pycard)));
		//             string name1=CStringA(CommonClass::username);
		//             string  name=CommonClass::Base64Encode(name1);
		//             mapValue.insert(make_pair("operatorName",name));
		std::stringstream body_data;
		body_data << '"' 
			<< "shopGuid=" << shopGuid
			<< "&cardID=" << cardID
			<< "&physicalCard=" << physicalCard
			<< "&operatorName=" << osl::Base64::encode(operatorName.c_str(), operatorName.length())
			<< '"';
		return DoHttpPost(s_pRunConfig->GetInsertCardInfoURL(), body_data.str(), listener);
	}

	bool UserCenter::SelectGroupMembership( const int firstRowNumber, const int lastRowNumber, const std::string& sortField, const std::string& sortType, const std::string& shopGuid, net::CURLWork::Listener* listener )
	{
		//             map<string,string> mapValue;
		//             mapValue.insert(make_pair("firstRowNumber","1"));
		//             mapValue.insert(make_pair("lastRowNumber","1"));
		//             mapValue.insert(make_pair("sortField",CommonClass::Base64Encode("ID")));
		//             mapValue.insert(make_pair("sortType","desc"));
		//             mapValue.insert(make_pair("shopGuid",CStringA(CommonClass::shopguid)));
		std::stringstream body_data;
		body_data << '"' 
			<< "firstRowNumber=" << firstRowNumber
			<< "&lastRowNumber=" << lastRowNumber
			<< "&sortField=" << sortField
			<< "&sortType=" << sortType
			<< "&shopGuid=" << shopGuid
			<< '"';
		return DoHttpPost(s_pRunConfig->GetSelectGroupMembershipURL(), body_data.str(), listener);
	}

	bool UserCenter::StoreMemberConsumption( const std::string& cardID, const std::string& memberGuid, const double storedValue, const std::string& shopGuid, const double cashmoney, const std::string& ticketGUID, const double OfferAmount, const std::string& code, const int HasDetails, const std::string& operaGuid, net::CURLWork::Listener* listener )
	{
		//             map<string,string> maptest;
		//             maptest.insert(make_pair("cardID", CmemberInfo.CmemberCard)); 
		//             maptest.insert(make_pair("memberGuid", CmemberInfo.CmemberGuid)); 
		//             double   f   =  CmemberInfo.paidAmount;
		//             CString   str; 
		//             str.Format(_T( "%.2f "),   f); 
		//             str.TrimRight( ' ');
		//             maptest.insert(make_pair("storedValue",CStringA(str))); 
		//             maptest.insert(make_pair("shopGuid",CStringA(CommonClass::shopguid)));
		//             maptest.insert(make_pair("cashmoney","0"));
		//             maptest.insert(make_pair("ticketGUID",CmemberInfo.ticketGuid));
		//             double   d= CmemberInfo.discountRate;
		//             CString   str1; 
		//             str1.Format(_T( "%.2f "),   d); 
		//             str1.TrimRight( ' ');
		//             maptest.insert(make_pair("OfferAmount",CStringA(str1)));
		//             maptest.insert(make_pair("code",CmemberInfo.dynamicCode));
		//             maptest.insert(make_pair("HasDetails",CommonClass::IntToString(isTrueProduct)));
		//             maptest.insert(make_pair("operaGuid",CStringA(CommonClass::userguid)));
		std::stringstream body_data;
		body_data << '"' 
			<< "cardID=" << cardID
			<< "&memberGuid=" << memberGuid
			<< "&storedValue=" << storedValue
			<< "&shopGuid=" << shopGuid
			<< "&cashmoney=" << cashmoney
			<< "&ticketGUID=" << ticketGUID
			<< "&OfferAmount=" << OfferAmount
			<< "&code=" << code
			<< "&HasDetails=" << HasDetails
			<< "&operaGuid=" << operaGuid
			<< '"';
		return DoHttpPost(s_pRunConfig->GetStoreMemberConsumptionURL(), body_data.str(), listener);
	}

	bool UserCenter::ConsumerMember( const std::string& Json1, const std::string& shopGuid, const int HasDetails, net::CURLWork::Listener* listener )
	{
		//             string StrJson=fast_writer.write(json_temp1);
		//             string  StrJosn1=CommonClass::Base64Encode(StrJson);
		//             map<string,string> maptest;
		//             maptest.insert(make_pair("Json1",StrJosn1)); 
		//             maptest.insert(make_pair("shopGuid", CStringA(CommonClass::shopguid))); 
		//             maptest.insert(make_pair("HasDetails", CommonClass::IntToString( isTrueProduct))); 
		std::stringstream body_data;
		body_data << '"' 
			<< "Json1=" << osl::Base64::encode(Json1.c_str(), Json1.length())
			<< "&shopGuid=" << shopGuid
			<< "&HasDetails=" << HasDetails
			<< '"';
		return DoHttpPost(s_pRunConfig->GetConsumerMemberURL(), body_data.str(), listener);
	}

	bool UserCenter::UnitedMemberConsumption( const std::string& cardID, const std::string& memberGuid, const double storedValue, const std::string& shopGuid, const double cashmoney, const std::string& ticketGUID, const double OfferAmount, const std::string& code, const int HasDetails, const std::string& operaGuid, net::CURLWork::Listener* listener )
	{
		//             map<string,string> maptest;
		//             maptest.insert(make_pair("cardID", CmemberInfo.CmemberCard)); 
		//             maptest.insert(make_pair("memberGuid", CmemberInfo.CmemberGuid)); 
		//             double   f   =  CmemberInfo.paidAmount;
		//             CString   str; 
		//             str.Format(_T( "%.2f"),f); 
		//             maptest.insert(make_pair("storedValue",CStringA(str))); 
		//             maptest.insert(make_pair("shopGuid",CStringA(CommonClass::shopguid)));
		//             maptest.insert(make_pair("cashmoney","0"));
		//             maptest.insert(make_pair("ticketGUID",CmemberInfo.ticketGuid));
		//             double   d= CmemberInfo.discountRate;
		//             CString   str1; 
		//             str1.Format(_T( "%.2f"),   d); 
		//             maptest.insert(make_pair("OfferAmount",CStringA(str1)));
		//             maptest.insert(make_pair("code",CmemberInfo.dynamicCode));
		//             maptest.insert(make_pair("HasDetails",CommonClass::IntToString(isTrueProduct)));
		//             maptest.insert(make_pair("operaGuid",CStringA(CommonClass::userguid)));
		std::stringstream body_data;
		body_data << '"' 
			<< "cardID=" << cardID
			<< "&memberGuid=" << memberGuid
			<< "&storedValue=" << storedValue
			<< "&shopGuid=" << shopGuid
			<< "&cashmoney=" << cashmoney
			<< "&ticketGUID=" << ticketGUID
			<< "&OfferAmount=" << OfferAmount
			<< "&code=" << code
			<< "&HasDetails=" << HasDetails
			<< "&operaGuid=" << operaGuid
			<< '"';
		return DoHttpPost(s_pRunConfig->GetUnitedMemberConsumptionURL(), body_data.str(), listener);
	}

	bool UserCenter::ConsumerNonMember( const std::string& Json1, net::CURLWork::Listener* listener )
	{
		//             map<string,string> maptest;
		//             string StrJson=fast_writer.write(json_temp);
		//             string  StrJosn1=CommonClass::Base64Encode(StrJson);
		//             list<Json::Value> m_listjson;
		//             string StrJosn2;
		//             list<UMS_OPR_MemuInfo> listmemu;
		//             Request webAccess;
		//             CString result;
		//             maptest.insert(make_pair( "Json1",StrJosn1)); 
		std::stringstream body_data;
		body_data << '"' 
			<< "Json1=" << osl::Base64::encode(Json1.c_str(), Json1.length())
			<< '"';
		return DoHttpPost(s_pRunConfig->GetConsumerNonMemberURL(), body_data.str(), listener);
	}

	bool UserCenter::InsertCommunicationInfo( const std::string& Json, net::CURLWork::Listener* listener )
	{
		//             string StrJson=fast_writer.write(json_temp);
		//             string  StrJosn1=CommonClass::Base64Encode(StrJson);
		//             map<string,string> maptest;
		//             maptest.insert(make_pair( "Json",StrJosn1));

		std::stringstream body_data;
		body_data << '"' 
			<< "Json=" << osl::Base64::encode(Json.c_str(), Json.length())
			<< '"';
		return DoHttpPost(s_pRunConfig->GetInsertCommunicationInfoURL(), body_data.str(), listener);
	}

}

