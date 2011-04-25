<?php

# Here is the Request XML we're trying to create for a valid request

/*
<S:Envelope xmlns:S="http://schemas.xmlsoap.org/soap/envelope/">
    <S:Header>
        <credentialsHolder>
            <password>qwerqreqeowruo1341</password>
            <userName>chip</userName>
        </credentialsHolder>
    </S:Header>
    <S:Body>
        <ns2:availabilitySearch xmlns:ns2="http://ws.resmarksystems.com/">
            <arg0>
                <activityNameIds>17</activityNameIds>
                <beginDate>2008-01-01T10:08:22.727-07:00</beginDate>
                <endDate>2008-12-31T10:08:22.730-07:00</endDate>
                <numAdults>0</numAdults>
                <numUnits>0</numUnits>
                <numYouths>0</numYouths>
                <onlineSearch>false</onlineSearch>
                <showNestedResults>true</showNestedResults>
                <showPackages>true</showPackages>
                <showPackageOnlyItems>true</showPackageOnlyItems>
                <upsell>false</upsell>
            </arg0>
        </ns2:availabilitySearch>
    </S:Body>
</S:Envelope>


<?xml version="1.0" encoding="UTF-8"?>
<SOAP-ENV:Envelope xmlns:SOAP-ENV="http://schemas.xmlsoap.org/soap/envelope/" xmlns:ns1="http://ws.resmarksystems.com/">
	<SOAP-ENV:Header>
		<credentialsHolder>
			<password>c6d3f9cdcd89188b37562b184b86e099</password>
			<userName>seth</userName>
		</credentialsHolder>
	</SOAP-ENV:Header>
	<SOAP-ENV:Body>
		<ns1:availabilitySearch/>
			<param1>2008-01-01T10:08:22.727-07:00</param1>
			<param2>2008-12-31T10:08:22.730-07:00</param2>
			<param3>0</param3>
			<param4>0</param4>
			<param5>0</param5>
			<param6>false</param6>
			<param7>true</param7>
			<param8>true</param8>
	</SOAP-ENV:Body>
</SOAP-ENV:Envelope>


*/
	
	//Declare our variables 
	$wsdl =  'http://staging.westernriver.com:8080/express/ReadOnlyWebServiceDispatcherService?WSDL';
	$password = 'ca425b88f047ce8ec45ee90e813ada91';	
	$user = 'anne';
	$header = new SoapVar("<credentialsHolder><password>$password</password><userName>$user</userName></credentialsHolder>", XSD_ANYXML, null, null, null, null);
	$parameters = array();
	$parameters[] = 4;
	
	//Set up the SOAP object
	$soapclient = new SoapClient($wsdl, array('exceptions' => true, 'trace' => true) );
	
	//Create a new SOAP header
	$objHeader_Session_Outside = new SoapHeader('http://schemas.xmlsoap.org/soap/envelope/', 'Header', $header);
	
	//Pass in the new header
	$soapclient->__setSoapHeaders(array($objHeader_Session_Outside));
	
	$searchParameters = array(
		'activityNameIds' => 17, 
		'beginDate' => '2008-01-01T10:08:22.727-07:00',
		'endDate' => '2008-12-31T10:08:22.730-07:00',
		'numAdults' => 1,
		'numYouths' => 0,
		'numUnits' => 0,
		'onlineSearch' => true,
		'showPackages' => true,
		'showPackageOnlyItems' => true,
		'showNestedResults' => true,
		'upsell' => false
	);

	//Make a request to the giveMeANumber function
	try 
	{
		$response = $soapclient->availabilitySearch(array('arg0' => $searchParameters));
	}
	catch(Exception $e) 
	{
		echo "oops, I got an exception...<br />";
		echo $e->getMessage();
	}
	

	/// debugging info:
	//echo "request: " . $soapclient->__getLastRequest() . "<br />";
	//echo "response: " . $soapclient->__getLastResponse() . "<br />";
	//
	//echo '<pre>';
	//print_r( $soapclient->__getFunctions() );
	//print_r( $soapclient->__getTypes() );
	//echo '</pre>';
	
	echo 'response...<br />';
	echo '<pre>';
	var_dump($response);
	echo '</pre>';
	exit;
?>