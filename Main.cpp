#include "gtest/gtest.h"
#include "gmock/gmock.h"

class SomeConnectDBCommand
{
};

class Connection
{
public:
    Connection()
    {}
    
    Connection(const char* data)
    {}

    bool operator==(const Connection& other) const
    {
        return true;
    }
};


class ClassResult
{
public:  
    bool operator==(const ClassResult) const
    {
        return true;
    }
};


class DBConnectionInterface
{
public:
    DBConnectionInterface()
    {}

    virtual ~DBConnectionInterface()
    {}

    virtual Connection open(const SomeConnectDBCommand& cmd) = 0;

    virtual bool close(const Connection& conn) = 0;

    virtual ClassResult execQuery(const Connection& conn, const SomeConnectDBCommand& cmd) = 0;
};


class DBConnection : public DBConnectionInterface
{
public:
    DBConnection()
    {}

    virtual ~DBConnection()
    {}

    virtual Connection open(const SomeConnectDBCommand& cmd) override
    {
        return Connection();
    }

    virtual bool close(const Connection& conn) override
    {
        return true;
    }

    virtual ClassResult execQuery(const Connection& conn, const SomeConnectDBCommand& cmd) override
    {
        return ClassResult();
    }
};

class ClassThatUsesDBInterface
{
public:
    ClassThatUsesDBInterface()
    {}

    virtual ~ClassThatUsesDBInterface()
    {}

    virtual Connection openConnection(const SomeConnectDBCommand& cmd) const = 0;

    virtual bool closeConnection(const Connection& conn) const = 0;


    virtual ClassResult useConnection(DBConnectionInterface* chanel, const SomeConnectDBCommand& cmd) = 0;
};

class ClassThatUsesDB : public ClassThatUsesDBInterface
{
public:
    ClassThatUsesDB()
    {}

    virtual ~ClassThatUsesDB()
    {}

    virtual Connection openConnection(const SomeConnectDBCommand& cmd) const override
    {
        return Connection();
    }

    virtual bool closeConnection(const Connection& conn) const override
    {
        return true;
    }

    virtual ClassResult useConnection(DBConnectionInterface* chanel, const SomeConnectDBCommand& cmd)
    {
        Connection conn = chanel->open(cmd);

        ClassResult responceData = chanel->execQuery(conn, cmd);

        return ClassResult();
    }
};


class MockExchangeDBConnection : public DBConnectionInterface
{
public:
    MOCK_METHOD(Connection, open, (const SomeConnectDBCommand& cmd), (override));
    MOCK_METHOD(bool, close, (const Connection& conn), (override));
    MOCK_METHOD(ClassResult, execQuery, (const Connection& conn, const SomeConnectDBCommand& cmd), (override));
};


class SomeTestSuite : public ::testing::Test
{
protected:
    void SetUp()
    {
        someConnection = new ClassThatUsesDB();
    }

    void TearDown()
    {
        delete someConnection;
    }

protected:
    ClassThatUsesDB* someConnection;
};


TEST_F(SomeTestSuite, testcase1)
{
    SomeConnectDBCommand cmd;

    Connection conn = someConnection->openConnection(cmd);

    Connection reference("MYSQL_CONN");

    ASSERT_EQ(conn, reference);
}


TEST_F(SomeTestSuite, testcase2)
{
    Connection conn;

    bool reference = true;

    bool responce = someConnection->closeConnection(conn);

    ASSERT_EQ(reference, responce);
}

TEST_F(SomeTestSuite, testcase3)
{
    SomeConnectDBCommand cmd;
    Connection conn;
    ClassResult result;

    MockExchangeDBConnection mconnect;

    EXPECT_CALL(mconnect, open).WillOnce(::testing::Return(conn));

    EXPECT_CALL(mconnect, close).WillRepeatedly(::testing::Return(true));

    EXPECT_CALL(mconnect, execQuery).WillOnce(::testing::Return(result));

    ClassResult responce = someConnection->useConnection(&mconnect, cmd);

    ASSERT_EQ(result, responce);
}


int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}