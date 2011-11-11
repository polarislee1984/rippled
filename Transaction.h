#ifndef __TRANSACTION__
#define __TRANSACTION__

#include "uint256.h"
#include "newcoin.pb.h"
#include <boost/shared_ptr.hpp>

/*
We could have made something that inherited from the protobuf transaction but this seemed simpler
*/

enum TransStatus
{
	INVALID,	// no valid signature, insufficient funds
	INCLUDED,	// added to the current ledger
	CONFLICTED,	// losing to a conflicting transaction
	COMMITTED,	// known to be in a ledger
	HELD,		// not valid now, maybe later
};


class Transaction
{
public:
	typedef boost::shared_ptr<Transaction> pointer;

private:
	uint256		mTransactionID;
	uint160		mAccountFrom, mAccountTo;
	uint64		mAmount;
	uint32		mFromAccountSeq, mSourceLedger, mIdent;
	std::vector<unsigned char> mSignature;

	uint32		mInLedger;
	TransStatus	mStatus;

public:
	Transaction();
	Transaction(const uint256 &id);
	Transaction(const std::vector<unsigned char> rawTransaction);
	Transaction(const std::string sqlReply);
	
	Transaction(Account &from, Account &to, uint64 amount, uint32 ident, uint32 ledger);
	bool Sign(const std::vector<unsigned char>& privKey);

	const std::string getSQL() const;
	bool checkSignature(void) const;

	const uint256& GetID() const { return mTransactionID; }
	const uint160& GetFromAccount() const { return mAccountFrom; }
	const uint160& GetToAccount() const { return mAccountTo; }
	uint64 GetAmount() const { return mAmount; }
	uint32 GetFromAccountSeq() const { return mFromAccountSeq; }
	uint32 GetSourceLedger() const { return mSourceLedger; }
	uint32 GetIdent() const { return mIdent; }
	const std::vector<unsigned char>& GetSignature() const { return mSignature; }
	uint32 GetLedger() const { return mInLedger; }
	TransStatus GetStatus() const { return mStatus; }

	void SetStatus(TransStatus st);	

	bool operator<(const Transaction &) const;
	bool operator>(const Transaction &) const;
	bool operator==(const Transaction &) const;
	bool operator!=(const Transaction &) const;
	bool operator<=(const Transaction &) const;
	bool operator>=(const Transaction &) const;
};

#endif
