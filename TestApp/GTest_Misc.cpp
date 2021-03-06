
#include "TestAppPch.h"

#if GTEST

using namespace Helium;

TEST(Foundation, FilePath)
{
    {
        String tempString;
        FilePath pathCopy;
        FilePath path( TXT( "C:/Users/Test/File.notext.ext" ) );
        HELIUM_TRACE( TraceLevels::Info, TXT( "path: %s\n" ), path.c_str() );

        pathCopy = path;
        tempString = pathCopy.Directory().c_str();
        pathCopy.Set( pathCopy.Directory() );
        HELIUM_ASSERT( tempString == pathCopy.c_str() );
        HELIUM_TRACE( TraceLevels::Info, TXT( "directory name: %s\n" ), *tempString );

        pathCopy = path;
        tempString = pathCopy.Filename().c_str();
        pathCopy.Set( pathCopy.Filename() );
        HELIUM_ASSERT( tempString == pathCopy.c_str() );
        HELIUM_TRACE( TraceLevels::Info, TXT( "filename: %s\n" ), *tempString );

        pathCopy = path;
        tempString = pathCopy.Basename().c_str();
        pathCopy.Set( pathCopy.Basename() );
        HELIUM_ASSERT( tempString == pathCopy.c_str() );
        HELIUM_TRACE( TraceLevels::Info, TXT( "base name: %s\n" ), *tempString );

        pathCopy = path;
        tempString = pathCopy.Extension().c_str();
        pathCopy.Set( pathCopy.Extension() );
        HELIUM_ASSERT( tempString == pathCopy.c_str() );
        HELIUM_TRACE( TraceLevels::Info, TXT( "extension: %s\n" ), *tempString );
    }

    {
        FilePath dataDirectory;
        HELIUM_VERIFY( FileLocations::GetDataDirectory( dataDirectory ) );
        HELIUM_TRACE( TraceLevels::Debug, TXT( "Data directory: %s\n" ), dataDirectory.c_str() );
        HELIUM_UNREF( dataDirectory );

        FilePath userDataDirectory;
        HELIUM_VERIFY( FileLocations::GetUserDataDirectory( userDataDirectory ) );
        HELIUM_TRACE( TraceLevels::Debug, TXT( "User data directory: %s\n" ), userDataDirectory.c_str() );
        HELIUM_UNREF( userDataDirectory );
    }

}

TEST(Platform, ThreadStartAndJoin)
{
    TestRunnable* pRunnable = new TestRunnable( String( TXT( "Thread test string..." ) ) );
    HELIUM_ASSERT( pRunnable );

    RunnableThread* pThread = new RunnableThread( pRunnable );
    HELIUM_ASSERT( pThread );
    HELIUM_VERIFY( pThread->Start( TXT( "Test Thread" ) ) );
    HELIUM_VERIFY( pThread->Join() );

    delete pThread;
    delete pRunnable;
}

TEST(DataStructures, String)
{
    String testString( TXT( "Test" ) );
    testString = TXT( 'T' );
    testString = TXT( "Test" );
    testString += TXT( "String" );
    testString += TXT( '0' );
    testString.Add( TXT( '1' ) );
    testString.Remove( 10 );
    testString.Insert( 4, TXT( '-' ) );
    HELIUM_ASSERT( testString == TXT( "Test-String1" ) );
    HELIUM_TRACE( TraceLevels::Debug, TXT( "%s\n" ), *testString );

    HELIUM_TRACE( TraceLevels::Debug, TXT( "\n" ) );
    HELIUM_TRACE( TraceLevels::Debug, TXT( "String length = %Iu\n" ), testString.GetSize() );
    HELIUM_TRACE( TraceLevels::Debug, TXT( "String capacity = %Iu\n" ), testString.GetCapacity() );
}

TEST(Framework, StackHeap)
{
    StackMemoryHeap<>& rStackHeap = ThreadLocalStackAllocator::GetMemoryHeap();
    StackMemoryHeap<>::Marker stackMarker( rStackHeap );

    int* pInt0 = static_cast< int* >( rStackHeap.Allocate( sizeof( int ) ) );
    *pInt0 = 7;

    stackMarker.Pop();

    int* pInt1 = static_cast< int* >( rStackHeap.Allocate( sizeof( int ) ) );
    *pInt1 = -3;

    HELIUM_TRACE( TraceLevels::Debug, TXT( "*pInt0 = %d; *pInt1 = %d\n" ), *pInt0, *pInt1 );

    rStackHeap.Free( pInt1 );
}

TEST(DataStructures, SortedSet)
{
    typedef SortedSet< int32_t > IntegerSetType;

    IntegerSetType integerSet;

    srand( static_cast< unsigned int >( time( NULL ) ) );
    for( size_t iteration = 0; iteration < 5000; ++iteration )
    {
        size_t setSize = integerSet.GetSize();
        if( !setSize || !( rand() & 0x1 ) )
        {
            IntegerSetType::ConstIterator iterator;
            int32_t value = rand();
            if( integerSet.Insert( iterator, value ) )
            {
                HELIUM_TRACE(
                    TraceLevels::Debug,
                    TXT( "@ size %" ) PRIuSZ TXT( ":\tInserting %" ) PRId32 TXT( "\n" ),
                    setSize,
                    value );
            }
        }
        else
        {
            size_t offset = ( static_cast< size_t >( rand() ) % setSize );
            IntegerSetType::Iterator iterator = integerSet.Begin();
            while( offset )
            {
                --offset;
                ++iterator;
            }

            HELIUM_TRACE(
                TraceLevels::Debug,
                TXT( "@ size %" ) PRIuSZ TXT( ":\tRemoving %" ) PRId32 TXT( "\n" ),
                setSize,
                *iterator );

            integerSet.Remove( iterator );
        }

        HELIUM_ASSERT( integerSet.Verify() );
    }
}

TEST(DataStructures, DynamicArray)
{
    {
        DynamicArray< int > intArray;
        intArray.Reserve( 3 );
        intArray.Add( 4 );
        //intArray.Add( 2 );
        HELIUM_VERIFY( intArray.New( 2 ) );
        intArray.Add( 8 );
        intArray.Add( 9 );
        intArray.Trim();

        PrintArrayInfo( TXT( "intArray" ), intArray );
    }

    {
        DynamicArray< NonTrivialClass > objectArray;
        objectArray.Reserve( 3 );
        objectArray.Add( NonTrivialClass( 4.3f ) );
        objectArray.Add( NonTrivialClass( 12.4f ) );
        objectArray.Add( NonTrivialClass( -3.9f ) );
        objectArray.Add( NonTrivialClass( 0.5f ) );
        objectArray.Add( NonTrivialClass( 2.9f ) );
        objectArray.Add( NonTrivialClass( -15.8f ) );
        objectArray.RemoveSwap( 1, 2 );
        objectArray.Trim();

        PrintArrayInfo( TXT( "objectArray" ), objectArray );
    }
}

TEST(DataStructures, Map) 
{
    Map< String, uint32_t > integerMap;
    Map< String, uint32_t >::Iterator mapIterator;

    integerMap[ String( TXT( "january" ) ) ] = 31;
    integerMap[ String( TXT( "february" ) ) ] = 28;
    integerMap[ String( TXT( "march" ) ) ] = 31;
    integerMap[ String( TXT( "april" ) ) ] = 30;
    integerMap[ String( TXT( "may" ) ) ] = 31;
    integerMap[ String( TXT( "june" ) ) ] = 30;
    integerMap[ String( TXT( "july" ) ) ] = 31;
    integerMap[ String( TXT( "august" ) ) ] = 31;
    integerMap[ String( TXT( "september" ) ) ] = 30;
    integerMap[ String( TXT( "october" ) ) ] = 31;

    integerMap[ String( TXT( "november" ) ) ] = 15;
    integerMap[ String( TXT( "november" ) ) ] = 30;

    bool bSuccess = integerMap.Insert( mapIterator, Pair< String, uint32_t >( String( TXT( "december" ) ), 31 ) );
    HELIUM_ASSERT( bSuccess );
    bSuccess = integerMap.Insert( mapIterator, Pair< String, uint32_t >( String( TXT( "december" ) ), 25 ) );
    HELIUM_ASSERT( !bSuccess );
    HELIUM_UNREF( bSuccess );

    HELIUM_TRACE( TraceLevels::Debug, TXT( "june -> %" ) PRIu32 TXT( "\n" ), integerMap[ String( TXT( "june" ) ) ] );

    HELIUM_TRACE( TraceLevels::Debug, TXT( "Map contents:\n" ) );

    Map< String, uint32_t >::Iterator endIterator = integerMap.End();
    for( mapIterator = integerMap.Begin(); mapIterator != endIterator; ++mapIterator )
    {
        HELIUM_TRACE(
            TraceLevels::Debug,
            TXT( "%s -> %" ) PRIu32 TXT( "\n" ),
            *mapIterator->First(),
            mapIterator->Second() );
    }
}

TEST(DataStructures, SortedMap)
{
    typedef SortedMap< String, uint32_t, StringCompareFunction > IntegerMapType;

    IntegerMapType integerMap;
    IntegerMapType::Iterator mapIterator;

    HELIUM_ASSERT( integerMap.Verify() );
    integerMap[ String( TXT( "january" ) ) ] = 31;
    HELIUM_ASSERT( integerMap.Verify() );
    integerMap[ String( TXT( "february" ) ) ] = 28;
    HELIUM_ASSERT( integerMap.Verify() );
    integerMap[ String( TXT( "march" ) ) ] = 31;
    HELIUM_ASSERT( integerMap.Verify() );
    integerMap[ String( TXT( "april" ) ) ] = 30;
    HELIUM_ASSERT( integerMap.Verify() );
    integerMap[ String( TXT( "may" ) ) ] = 31;
    HELIUM_ASSERT( integerMap.Verify() );
    integerMap[ String( TXT( "june" ) ) ] = 30;
    HELIUM_ASSERT( integerMap.Verify() );
    integerMap[ String( TXT( "july" ) ) ] = 31;
    HELIUM_ASSERT( integerMap.Verify() );
    integerMap[ String( TXT( "august" ) ) ] = 31;
    HELIUM_ASSERT( integerMap.Verify() );
    integerMap[ String( TXT( "september" ) ) ] = 30;
    HELIUM_ASSERT( integerMap.Verify() );
    integerMap[ String( TXT( "october" ) ) ] = 31;
    HELIUM_ASSERT( integerMap.Verify() );

    integerMap[ String( TXT( "november" ) ) ] = 15;
    HELIUM_ASSERT( integerMap.Verify() );
    integerMap[ String( TXT( "november" ) ) ] = 30;
    HELIUM_ASSERT( integerMap.Verify() );

    bool bSuccess = integerMap.Insert( mapIterator, Pair< String, uint32_t >( String( TXT( "december" ) ), 31 ) );
    HELIUM_ASSERT( bSuccess );
    HELIUM_ASSERT( integerMap.Verify() );
    bSuccess = integerMap.Insert( mapIterator, Pair< String, uint32_t >( String( TXT( "december" ) ), 25 ) );
    HELIUM_ASSERT( !bSuccess );
    HELIUM_UNREF( bSuccess );

    HELIUM_TRACE( TraceLevels::Debug, TXT( "SortedMap contents:\n" ) );

    IntegerMapType::Iterator mapEnd = integerMap.End();
    for( mapIterator = integerMap.Begin(); mapIterator != mapEnd; ++mapIterator )
    {
        HELIUM_TRACE(
            TraceLevels::Debug,
            TXT( "%s -> %" ) PRIu32 TXT( "\n" ),
            *mapIterator->First(),
            mapIterator->Second() );
    }

    HELIUM_VERIFY( integerMap.Remove( String( TXT( "march" ) ) ) );
    HELIUM_ASSERT( integerMap.Verify() );
}

TEST(Framework, Sockets) 
{
    //pmd - Added UDP/TCP socket demo code
    HELIUM_TRACE( TraceLevels::Debug, TXT("Testing Sockets"));
    Helium::InitializeSockets();
    {
        HELIUM_TRACE( TraceLevels::Debug, TXT(" - UDP"));
        // Get the local host information
        hostent* localhost = gethostbyname("");
        char* local_ip = inet_ntoa (*(struct in_addr *)*localhost->h_addr_list);

        Helium::Socket peer1;
        peer1.Create(Helium::SocketProtocols::Udp);
        peer1.Bind(45554);

        Helium::Socket peer2;
        peer2.Create(Helium::SocketProtocols::Udp);
        peer2.Bind(45555);

        uint32_t wrote;
        peer1.Write("test_data", 10, wrote, local_ip, 45555);

        char buffer[256];
        sockaddr_in peer_addr;

        uint32_t read;
        peer2.Read(buffer, 256, read, &peer_addr);

        peer1.Close();
        peer2.Close();
    }

    {
        HELIUM_TRACE( TraceLevels::Debug, TXT(" - TCP"));

        // Get the local host information
        hostent* localhost = gethostbyname("");
        char* local_ip = inet_ntoa (*(struct in_addr *)*localhost->h_addr_list);

        Helium::Socket peer1;
        peer1.Create(Helium::SocketProtocols::Tcp);
        peer1.Bind(45554);
        peer1.Listen();

        Helium::Socket peer2;
        peer2.Create(Helium::SocketProtocols::Tcp);
        peer2.Connect( 45555 );

        sockaddr_in peer_addr;
        Helium::Socket peer1_new_client;
        peer1_new_client.Create(SocketProtocols::Tcp);
        peer1_new_client.Accept(peer1, &peer_addr);

        uint32_t wrote;
        peer1_new_client.Write("test_data", 10, wrote);

        char buffer[256];

        uint32_t read;
        peer2.Read(buffer, 256, read);
        peer1.Close();
        peer2.Close();
        peer1_new_client.Close();
    }

    Helium::CleanupSockets();
}

TEST(Engine, PackageObjectTest)
{
    {
        AssetPath testPath;
        HELIUM_VERIFY( testPath.Set( HELIUM_PACKAGE_PATH_CHAR_STRING TXT( "EngineTest" ) HELIUM_OBJECT_PATH_CHAR_STRING TXT( "TestObject" ) ) );

        AssetPtr spObject;
        HELIUM_VERIFY( gAssetLoader->LoadObject( testPath, spObject ) );
        HELIUM_ASSERT( spObject );

        Package* pTestPackageCast = Reflect::SafeCast< Package >( spObject.Get() );
        HELIUM_ASSERT( !pTestPackageCast );
        HELIUM_UNREF( pTestPackageCast );

        // The following line should not compile...
        //            Animation* pTestAnimationCast = Reflect::SafeCast< Animation >( pTestPackageCast );
        //            HELIUM_UNREF( pTestAnimationCast );

        Asset* pTestObjectCast = Reflect::SafeCast< Asset >( spObject.Get() );
        HELIUM_ASSERT( pTestObjectCast );
        HELIUM_UNREF( pTestObjectCast );
    }
}

#endif