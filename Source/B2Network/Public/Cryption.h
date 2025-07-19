#pragma once

#include <array>
#include <iostream>
//#include <sodium.h>

namespace b2network
{
	//using KeyType = std::array<uint8, crypto_stream_chacha20_KEYBYTES>;
	//using NonceType = std::array<uint8, crypto_stream_chacha20_NONCEBYTES>;
		using KeyType = std::array<uint8, 32>;
	using NonceType = std::array<uint8, 12>;

	B2NETWORK_API std::string hexify(const uint8* v, size_t len);
	B2NETWORK_API bool unhexify(uint8 *out, size_t len, const std::string& hex_str);

	class B2NETWORK_API Key
	{
	public:
		Key();
		~Key() = default;

		KeyType		GetPublicKey() const;
		void		MakeSharedKeyForClient(KeyType const&);
		void		MakeSharedKeyForServer(KeyType const&);

		KeyType		GetRealKey() const
		{
			return realKey;
		}

		NonceType	GetNonceKey() const
		{
			return nonceKey;
		}

		std::string GetPrivateKeyToString() const;
		std::string GetPublicKeyToString() const;

/* comment out this code block to use later
		int32 my_crypto_scalarmult_base(unsigned char *, const unsigned char *);
*/

	private:
		KeyType		privateKey;
		KeyType		publicKey;
		KeyType		realKey;
		NonceType	nonceKey;
	};

	class B2NETWORK_API ICrypto
	{
	public:
		virtual ~ICrypto() {}

		virtual bool Encrypt(unsigned char*, uint32) = 0;
		virtual bool Decrypt(unsigned char*, uint32) = 0;
	};

	class B2NETWORK_API Dummy : public ICrypto
	{
	public:
		virtual bool Encrypt(unsigned char*, uint32) override { return true; }
		virtual bool Decrypt(unsigned char*, uint32) override { return true; }
	};

	class B2NETWORK_API ChaCha20 : public ICrypto
	{
	public:
		explicit ChaCha20(const Key& key) : myKey(key) {}

		virtual bool Encrypt(unsigned char*, uint32) override;
		virtual bool Decrypt(unsigned char*, uint32) override;

	private:
		Key		myKey;
	};
}