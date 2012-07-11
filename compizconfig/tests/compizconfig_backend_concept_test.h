#ifndef _COMPIZCONFIG_CCS_BACKEND_CONCEPT_TEST
#define _COMPIZCONFIG_CCS_BACKEND_CONCEPT_TEST

#include <list>

#include <boost/variant.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <ccs-backend.h>
#include <ccs.h>

#include <compizconfig_ccs_plugin_mock.h>
#include <compizconfig_ccs_setting_mock.h>
#include <compizconfig_ccs_context_mock.h>

using ::testing::Eq;

class CCSListWrapper :
    boost::noncopyable
{
    public:

	typedef boost::shared_ptr <CCSListWrapper> Ptr;

	CCSListWrapper (CCSSettingValueList list, bool freeItems) :
	    mList (list),
	    mFreeItems (freeItems)
	{
	}

	operator CCSSettingValueList ()
	{
	    return mList;
	}

	operator CCSSettingValueList () const
	{
	    return mList;
	}

	~CCSListWrapper ()
	{
	    ccsSettingValueListFree (mList, mFreeItems ? TRUE : FALSE);
	}
    private:

	CCSSettingValueList mList;
	bool		    mFreeItems;
};

typedef boost::variant <bool,
			int,
			float,
			const char *,
			CCSSettingColorValue,
			CCSSettingKeyValue,
			CCSSettingButtonValue,
			unsigned int,
			CCSListWrapper::Ptr> VariantTypes;

class CCSBackendConceptTestEnvironmentInterface
{
    public:

	virtual ~CCSBackendConceptTestEnvironmentInterface () {};
	virtual CCSBackend * SetUp () = 0;
	virtual void TearDown (CCSBackend *) = 0;
	virtual void WriteBoolAtKey (const std::string &plugin,
				       const std::string &key,
				       const VariantTypes &value) = 0;
	virtual void WriteIntegerAtKey (const std::string &plugin,
					const std::string &key,
					const VariantTypes &value) = 0;
	virtual void WriteFloatAtKey (const std::string &plugin,
				      const std::string &key,
				      const VariantTypes &value) = 0;
	virtual void WriteStringAtKey (const std::string &plugin,
				       const std::string &key,
				       const VariantTypes &value) = 0;
	virtual void WriteColorAtKey (const std::string &plugin,
				       const std::string &key,
				       const VariantTypes &value) = 0;
	virtual void WriteKeyAtKey (const std::string &plugin,
				       const std::string &key,
				       const VariantTypes &value) = 0;
	virtual void WriteButtonAtKey (const std::string &plugin,
				       const std::string &key,
				       const VariantTypes &value) = 0;
	virtual void WriteEdgeAtKey (const std::string &plugin,
				       const std::string &key,
				       const VariantTypes &value) = 0;
	virtual void WriteMatchAtKey (const std::string &plugin,
				      const std::string &key,
				      const VariantTypes &value) = 0;
	virtual void WriteBellAtKey (const std::string &plugin,
				       const std::string &key,
				       const VariantTypes &value) = 0;
	virtual void WriteListAtKey (const std::string &plugin,
				     const std::string &key,
				     const VariantTypes &value) = 0;

	virtual Bool ReadBoolAtKey (const std::string &plugin,
				       const std::string &key) = 0;
	virtual int ReadIntegerAtKey (const std::string &plugin,
					const std::string &key) = 0;
	virtual float ReadFloatAtKey (const std::string &plugin,
				      const std::string &key) = 0;
	virtual const char * ReadStringAtKey (const std::string &plugin,
				       const std::string &key) = 0;
	virtual CCSSettingColorValue ReadColorAtKey (const std::string &plugin,
				       const std::string &key) = 0;
	virtual CCSSettingKeyValue ReadKeyAtKey (const std::string &plugin,
				       const std::string &key) = 0;
	virtual CCSSettingButtonValue ReadButtonAtKey (const std::string &plugin,
				       const std::string &key) = 0;
	virtual unsigned int ReadEdgeAtKey (const std::string &plugin,
				       const std::string &key) = 0;
	virtual const char * ReadMatchAtKey (const std::string &plugin,
				      const std::string &key) = 0;
	virtual Bool ReadBellAtKey (const std::string &plugin,
				       const std::string &key) = 0;
	virtual CCSSettingValueList ReadListAtKey (const std::string &plugin,
				     const std::string &key) = 0;
};

namespace
{
Bool boolToBool (bool v) { return v ? TRUE : FALSE; }

void SetIntReadExpectation (CCSSettingGMock *gmock, const VariantTypes &value)
{
    EXPECT_CALL (*gmock, setInt (boost::get <int> (value), _));
}

void SetBoolReadExpectation (CCSSettingGMock *gmock, const VariantTypes &value)
{
    EXPECT_CALL (*gmock, setBool (boolToBool (boost::get <bool> (value)), _));
}

void SetBellReadExpectation (CCSSettingGMock *gmock, const VariantTypes &value)
{
    EXPECT_CALL (*gmock, setBell (boolToBool (boost::get <bool> (value)), _));
}

void SetFloatReadExpectation (CCSSettingGMock *gmock, const VariantTypes &value)
{
    EXPECT_CALL (*gmock, setFloat (boost::get <float> (value), _));
}

void SetStringReadExpectation (CCSSettingGMock *gmock, const VariantTypes &value)
{
    EXPECT_CALL (*gmock, setString (Eq (std::string (boost::get <const char *> (value))), _));
}

void SetMatchReadExpectation (CCSSettingGMock *gmock, const VariantTypes &value)
{
    EXPECT_CALL (*gmock, setMatch (Eq (std::string (boost::get <const char *> (value))), _));
}

void SetColorReadExpectation (CCSSettingGMock *gmock, const VariantTypes &value)
{
    EXPECT_CALL (*gmock, setColor (_, _)); // can't match
}

void SetKeyReadExpectation (CCSSettingGMock *gmock, const VariantTypes &value)
{
    EXPECT_CALL (*gmock, setKey (_, _)); // can't match
}

void SetButtonReadExpectation (CCSSettingGMock *gmock, const VariantTypes &value)
{
    EXPECT_CALL (*gmock, setButton (_, _)); // can't match
}

void SetEdgeReadExpectation (CCSSettingGMock *gmock, const VariantTypes &value)
{
    EXPECT_CALL (*gmock, setEdge (boost::get <unsigned int> (value), _));
}

void SetListReadExpectation (CCSSettingGMock *gmock, const VariantTypes &value)
{
    EXPECT_CALL (*gmock, setList (_, _)); // can't match
}

}

class CCSBackendConceptTestParamInterface
{
    public:

	typedef boost::shared_ptr <CCSBackendConceptTestParamInterface> Ptr;

	typedef boost::function <void (const std::string &plugin,
				       const std::string &keyname,
				       const VariantTypes &value)> NativeWriteMethod;
	typedef boost::function <void (CCSSettingGMock *,
				       const VariantTypes &)> SetExpectation;

	virtual CCSBackendConceptTestEnvironmentInterface * testEnv () = 0;
	virtual VariantTypes & value () = 0;
	virtual NativeWriteMethod & nativeWrite () = 0;
	virtual CCSSettingType & type () = 0;
	virtual std::string & keyname () = 0;
	virtual SetExpectation & setExpectation () = 0;
	virtual std::string & what () = 0;
};

template <typename I>
class CCSBackendConceptTestParam :
    public CCSBackendConceptTestParamInterface
{
    public:

	typedef boost::shared_ptr <CCSBackendConceptTestParam <I> > Ptr;

	CCSBackendConceptTestParam (CCSBackendConceptTestEnvironmentInterface *testEnv,
				    const VariantTypes &value,
				    const NativeWriteMethod &write,
				    const CCSSettingType &type,
				    const std::string &keyname,
				    const SetExpectation &setExpectation,
				    const std::string &what) :
	    mTestEnv (testEnv),
	    mValue (value),
	    mNativeWrite (write),
	    mType (type),
	    mKeyname (keyname),
	    mSetExpectation (setExpectation),
	    mWhat (what)
	{
	}

	CCSBackendConceptTestEnvironmentInterface * testEnv () { return mTestEnv; }
	VariantTypes & value () { return mValue; }
	CCSBackendConceptTestParamInterface::NativeWriteMethod & nativeWrite () { return mNativeWrite; }
	CCSSettingType & type () { return mType; }
	std::string & keyname () { return mKeyname; }
	CCSBackendConceptTestParamInterface::SetExpectation & setExpectation () { return mSetExpectation; }
	std::string & what () { return mWhat; }

    private:

	CCSBackendConceptTestEnvironmentInterface *mTestEnv;
	VariantTypes mValue;
	NativeWriteMethod mNativeWrite;
	CCSSettingType mType;
	std::string mKeyname;
	SetExpectation mSetExpectation;
	std::string mWhat;

};

class CCSBackendConformanceTest :
    public ::testing::TestWithParam <CCSBackendConceptTestParamInterface::Ptr>
{
    public:

	CCSBackend * GetBackend ()
	{
	    return mBackend;
	}

	void SetUp ()
	{
	    mBackend = GetParam ()->testEnv ()->SetUp ();
	}

	void TearDown ()
	{
	    CCSBackendConformanceTest::GetParam ()->testEnv ()->TearDown (mBackend);

	    for (std::list <CCSContext *>::iterator it = mSpawnedContexts.begin ();
		 it != mSpawnedContexts.end ();
		 it++)
		ccsFreeMockContext (*it);

	    for (std::list <CCSPlugin *>::iterator it = mSpawnedPlugins.begin ();
		 it != mSpawnedPlugins.end ();
		 it++)
		ccsFreeMockPlugin (*it);

	    for (std::list <CCSSetting *>::iterator it = mSpawnedSettings.begin ();
		 it != mSpawnedSettings.end ();
		 it++)
		ccsFreeMockSetting (*it);
	}

    protected:

	CCSContext *
	SpawnContext ()
	{
	    CCSContext *context = ccsMockContextNew ();
	    mSpawnedContexts.push_back (context);
	    return context;
	}

	CCSPlugin *
	SpawnPlugin (const std::string &name = "")
	{
	    CCSPlugin *plugin = ccsMockPluginNew ();
	    mSpawnedPlugins.push_back (plugin);

	    CCSPluginGMock *gmockPlugin = (CCSPluginGMock *) ccsObjectGetPrivate (plugin);

	    if (!name.empty ())
		EXPECT_CALL (*gmockPlugin, getName ()).WillRepeatedly (Return ((char *) name.c_str ()));

	    return plugin;
	}

	CCSSetting *
	SpawnSetting (const std::string &name = "",
		      CCSSettingType	type = TypeNum,
		      CCSPlugin		*plugin = NULL)
	{
	    CCSSetting *setting = ccsMockSettingNew ();
	    mSpawnedSettings.push_back (setting);

	    CCSSettingGMock *gmockSetting = (CCSSettingGMock *) ccsObjectGetPrivate (setting);

	    if (!name.empty ())
		EXPECT_CALL (*gmockSetting, getName ()).WillRepeatedly (Return ((char *) name.c_str ()));

	    if (type != TypeNum)
		EXPECT_CALL (*gmockSetting, getType ()).WillRepeatedly (Return (type));

	    if (plugin)
		EXPECT_CALL (*gmockSetting, getParent ()).WillRepeatedly (Return (plugin));

	    return setting;
	}

    private:

	std::list <CCSContext *> mSpawnedContexts;
	std::list <CCSPlugin  *> mSpawnedPlugins;
	std::list <CCSSetting *> mSpawnedSettings;

	CCSBackend *mBackend;
};

namespace compizconfig
{
namespace test
{
namespace impl
{

Bool boolValues[] = { TRUE, FALSE, TRUE };
int intValues[] = { 1, 2, 3 };
float floatValues[] = { 1.0, 2.0, 3.0 };
const char * stringValues[] = { "foo", "grill", "bar" };

CCSSettingColorValue colorValues[3] = { { .color = { 100, 200, 300, 100 } },
					{ .color = { 50, 100, 200, 300 } },
					{ .color = { 10, 20, 30, 40 } }
				      };

CCSSettingKeyValue keyValue = { (1 << 0) | (1 << 1),
				1 };

CCSSettingButtonValue buttonValue = { (1 << 0) | (1 << 1),
				      1,
				      (1 << 1) };
}

template <typename I>
::testing::internal::ParamGenerator<typename CCSBackendConceptTestParamInterface::Ptr>
GenerateTestingParametersForBackendInterface ()
{
    static I interface;
    static CCSBackendConceptTestEnvironmentInterface *backendEnv = &interface;

    typedef CCSBackendConceptTestParam<I> ConceptParam;

    static typename CCSBackendConceptTestParamInterface::Ptr testParam[] =
    {
	boost::make_shared <ConceptParam> (backendEnv,
					   VariantTypes (1),
					   boost::bind (&CCSBackendConceptTestEnvironmentInterface::WriteIntegerAtKey, backendEnv, _1, _2, _3),
					   TypeInt,
					   "integer_setting",
					   boost::bind (SetIntReadExpectation, _1, _2),
					   "TestRetreiveInt"),
	boost::make_shared <ConceptParam> (backendEnv,
					   VariantTypes (true),
					   boost::bind (&CCSBackendConceptTestEnvironmentInterface::WriteBoolAtKey, backendEnv, _1, _2, _3),
					   TypeBool,
					   "boolean_setting",
					   boost::bind (SetBoolReadExpectation, _1, _2),
					   "TestRetreiveBool"),
	boost::make_shared <ConceptParam> (backendEnv,
					   VariantTypes (static_cast <float> (3.0)),
					   boost::bind (&CCSBackendConceptTestEnvironmentInterface::WriteFloatAtKey, backendEnv, _1, _2, _3),
					   TypeFloat,
					   "float_setting",
					   boost::bind (SetFloatReadExpectation, _1, _2),
					   "TestRetreiveFloat"),
	boost::make_shared <ConceptParam> (backendEnv,
					   VariantTypes (static_cast <const char *> ("foo")),
					   boost::bind (&CCSBackendConceptTestEnvironmentInterface::WriteStringAtKey, backendEnv, _1, _2, _3),
					   TypeString,
					   "string_setting",
					   boost::bind (SetStringReadExpectation, _1, _2),
					   "TestRetreiveString"),
	boost::make_shared <ConceptParam> (backendEnv,
					   VariantTypes (static_cast <const char *> ("foo=bar")),
					   boost::bind (&CCSBackendConceptTestEnvironmentInterface::WriteMatchAtKey, backendEnv, _1, _2, _3),
					   TypeMatch,
					   "match_setting",
					   boost::bind (SetMatchReadExpectation, _1, _2),
					   "TestRetreiveMatch"),
	boost::make_shared <ConceptParam> (backendEnv,
					   VariantTypes (true),
					   boost::bind (&CCSBackendConceptTestEnvironmentInterface::WriteBellAtKey, backendEnv, _1, _2, _3),
					   TypeBell,
					   "bell_setting",
					   boost::bind (SetBellReadExpectation, _1, _2),
					   "TestRetreiveBell"),
	boost::make_shared <ConceptParam> (backendEnv,
					   VariantTypes (impl::colorValues[0]),
					   boost::bind (&CCSBackendConceptTestEnvironmentInterface::WriteColorAtKey, backendEnv, _1, _2, _3),
					   TypeColor,
					   "color_setting",
					   boost::bind (SetColorReadExpectation, _1, _2),
					   "TestRetreiveColor"),
	boost::make_shared <ConceptParam> (backendEnv,
					   VariantTypes (impl::keyValue),
					   boost::bind (&CCSBackendConceptTestEnvironmentInterface::WriteKeyAtKey, backendEnv, _1, _2, _3),
					   TypeKey,
					   "key_setting",
					   boost::bind (SetKeyReadExpectation, _1, _2),
					   "TestRetreiveKey"),
	boost::make_shared <ConceptParam> (backendEnv,
					   VariantTypes (impl::buttonValue),
					   boost::bind (&CCSBackendConceptTestEnvironmentInterface::WriteButtonAtKey, backendEnv, _1, _2, _3),
					   TypeButton,
					   "button_setting",
					   boost::bind (SetButtonReadExpectation, _1, _2),
					   "TestRetreiveButton"),
	boost::make_shared <ConceptParam> (backendEnv,
					   VariantTypes (static_cast <unsigned int> (1)),
					   boost::bind (&CCSBackendConceptTestEnvironmentInterface::WriteEdgeAtKey, backendEnv, _1, _2, _3),
					   TypeEdge,
					   "edge_setting",
					   boost::bind (SetEdgeReadExpectation, _1, _2),
					   "TestRetreiveEdge"),
	boost::make_shared <ConceptParam> (backendEnv,
					   VariantTypes (boost::make_shared <CCSListWrapper> (ccsGetValueListFromIntArray (impl::intValues,
															   sizeof (impl::intValues) / sizeof (impl::intValues[0]),
															   NULL), false)),
					   boost::bind (&CCSBackendConceptTestEnvironmentInterface::WriteListAtKey, backendEnv, _1, _2, _3),
					   TypeList,
					   "int_list_setting",
					   boost::bind (SetListReadExpectation, _1, _2),
					   "TestRetreiveListInt"),
	boost::make_shared <ConceptParam> (backendEnv,
					   VariantTypes (boost::make_shared <CCSListWrapper> (ccsGetValueListFromFloatArray (impl::floatValues,
															     sizeof (impl::floatValues) / sizeof (impl::floatValues[0]),
															     NULL), false)),
					   boost::bind (&CCSBackendConceptTestEnvironmentInterface::WriteListAtKey, backendEnv, _1, _2, _3),
					   TypeList,
					   "float_list_setting",
					   boost::bind (SetListReadExpectation, _1, _2),
					   "TestRetreiveListInt"),
	boost::make_shared <ConceptParam> (backendEnv,
					   VariantTypes (boost::make_shared <CCSListWrapper> (ccsGetValueListFromBoolArray (impl::boolValues,
															   sizeof (impl::boolValues) / sizeof (impl::boolValues[0]),
															   NULL), false)),
					   boost::bind (&CCSBackendConceptTestEnvironmentInterface::WriteListAtKey, backendEnv, _1, _2, _3),
					   TypeList,
					   "bool_list_setting",
					   boost::bind (SetListReadExpectation, _1, _2),
					   "TestRetreiveListBool"),
	boost::make_shared <ConceptParam> (backendEnv,
					   VariantTypes (boost::make_shared <CCSListWrapper> (ccsGetValueListFromStringArray (impl::stringValues,
															      sizeof (impl::stringValues) / sizeof (impl::stringValues[0]),
															      NULL), false)),
					   boost::bind (&CCSBackendConceptTestEnvironmentInterface::WriteListAtKey, backendEnv, _1, _2, _3),
					   TypeList,
					   "string_list_setting",
					   boost::bind (SetListReadExpectation, _1, _2),
					   "TestRetreiveListString"),
	boost::make_shared <ConceptParam> (backendEnv,
					   VariantTypes (boost::make_shared <CCSListWrapper> (ccsGetValueListFromColorArray (impl::colorValues,
															     sizeof (impl::colorValues) / sizeof (impl::colorValues[0]),
															     NULL), false)),
					   boost::bind (&CCSBackendConceptTestEnvironmentInterface::WriteListAtKey, backendEnv, _1, _2, _3),
					   TypeList,
					   "color_list_setting",
					   boost::bind (SetListReadExpectation, _1, _2),
					   "TestRetreiveListColor")
    };

    return ::testing::ValuesIn (testParam);
}
}
}

TEST_P (CCSBackendConformanceTest, TestReadValue)
{
    SCOPED_TRACE (CCSBackendConformanceTest::GetParam ()->what ());

    std::string pluginName ("plugin");
    const std::string &settingName (GetParam ()->keyname ());
    const VariantTypes &VALUE (GetParam ()->value ());

    CCSContext *context = CCSBackendConformanceTest::SpawnContext ();
    CCSPlugin *plugin = CCSBackendConformanceTest::SpawnPlugin (pluginName);
    CCSSetting *setting = CCSBackendConformanceTest::SpawnSetting (settingName, GetParam ()->type (), plugin);

    CCSSettingGMock *gmockSetting = (CCSSettingGMock *) ccsObjectGetPrivate (setting);

    CCSBackendConformanceTest::GetParam ()->nativeWrite () (pluginName, settingName, VALUE);
    CCSBackendConformanceTest::GetParam ()->setExpectation () (gmockSetting, VALUE);

    ccsBackendReadSetting (CCSBackendConformanceTest::GetBackend (), context, setting);
}

#endif

