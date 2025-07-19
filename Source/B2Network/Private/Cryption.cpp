#include "Cryption.h"
#include "B2NetworkPlugin.h"


#include "LogManager.h"

namespace b2network
{
#if B2_BUILD_LEVEL < B2_BUILD_LEVEL_QA // !B2_BUILD_LEVEL_LIVE, !!B2_BUILD_LEVEL_QA
	static auto logger = LogManager::GetLogger("Cryption");
#endif

	std::string hexify(const uint8* v, size_t len)
	{
		std::string s;
		//s.resize(len * 2 + 1);
		//::sodium_bin2hex(&s[0], s.size(), v, len);
		//s.resize(len * 2);
		return s;
	}

	bool unhexify(uint8* out, size_t len, const std::string& hex_str)
	{
		//if (hex_str.size() != 64) 
			return false;

		//return 0 == ::sodium_hex2bin(out,
		//	len,
		//	hex_str.data(),
		//	hex_str.size(),
		//	nullptr, nullptr, nullptr);
	}

	Key::Key()
	{
		return;
		
/* comment out this code block to use later
#if !(PLATFORM_ANDROID || PLATFORM_IOS)
		FPlatformMisc::LowLevelOutputDebugString(TEXT("Force to use constant private and public keys..."));

		static const std::string str_private = "7783cef636abec3d8e817d4cfb104bf110e6f3515d3ef1779203e2c7b2e63e90";
		static const std::string str_public  = "10cd0dbc70a49460bdee228423f0b12091718de3850711085a131c53e309eb75";

		unhexify(&privateKey[0], crypto_stream_chacha20_KEYBYTES, str_private);
		unhexify(&publicKey[0], crypto_stream_chacha20_KEYBYTES, str_public);

		FPlatformMisc::LowLevelOutputDebugStringf(TEXT("Client Generated Private Key : %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d"),
			privateKey[0], privateKey[1], privateKey[2], privateKey[3], privateKey[4], privateKey[5], privateKey[6], privateKey[7],
			privateKey[8], privateKey[9], privateKey[10], privateKey[11], privateKey[12], privateKey[13], privateKey[14], privateKey[15],
			privateKey[16], privateKey[17], privateKey[18], privateKey[19], privateKey[20], privateKey[21], privateKey[22], privateKey[23],
			privateKey[24], privateKey[25], privateKey[26], privateKey[27], privateKey[28], privateKey[29], privateKey[30], privateKey[31]
		);

		FPlatformMisc::LowLevelOutputDebugStringf(TEXT("Client Generated Public Key : %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d"),
			publicKey[0], publicKey[1], publicKey[2], publicKey[3], publicKey[4], publicKey[5], publicKey[6], publicKey[7],
			publicKey[8], publicKey[9], publicKey[10], publicKey[11], publicKey[12], publicKey[13], publicKey[14], publicKey[15],
			publicKey[16], publicKey[17], publicKey[18], publicKey[19], publicKey[20], publicKey[21], publicKey[22], publicKey[23],
			publicKey[24], publicKey[25], publicKey[26], publicKey[27], publicKey[28], publicKey[29], publicKey[30], publicKey[31]
		);
#else
		check(crypto_scalarmult_SCALARBYTES == crypto_stream_chacha20_KEYBYTES);
		::randombytes_buf(&privateKey[0], crypto_stream_chacha20_KEYBYTES);
		check(my_crypto_scalarmult_base(&publicKey[0], &privateKey[0]) == 0);
#endif
*/
	}

	KeyType Key::GetPublicKey() const
	{
		return publicKey;
	}

	void Key::MakeSharedKeyForClient(const KeyType& ServerPublicKey)
	{
		//::memcpy(&realKey[0], &ServerPublicKey[0], crypto_stream_chacha20_KEYBYTES);
		//::memcpy(&nonceKey[0], &ServerPublicKey[0], crypto_stream_chacha20_NONCEBYTES);
/* comment out this code block to use later
		KeyType sharedKey;
		std::array<uint8, crypto_hash_sha512_BYTES>	hashKey;
		crypto_hash_sha512_state h;

		check(b2network::crypto_scalarmult(&sharedKey[0], &privateKey[0], &ServerPublicKey[0]) == 0);

		::crypto_hash_sha512_init(&h);
		::crypto_hash_sha512_update(&h, &sharedKey[0], sizeof sharedKey);
		::crypto_hash_sha512_update(&h, &publicKey[0], sizeof publicKey);
		::crypto_hash_sha512_update(&h, &ServerPublicKey[0], sizeof ServerPublicKey);
		::crypto_hash_sha512_final(&h, &hashKey[0]);

		::memcpy(&realKey[0], &hashKey[0], crypto_stream_chacha20_KEYBYTES);
		::memcpy(&nonceKey[0], &hashKey[crypto_stream_chacha20_KEYBYTES], crypto_stream_chacha20_NONCEBYTES);
*/
	}

	void Key::MakeSharedKeyForServer(const KeyType& ClientPublicKey)
	{
/* comment out this code block to use later
		KeyType sharedKey;
		std::array<uint8, crypto_hash_sha512_BYTES>	hashKey;
		crypto_hash_sha512_state h;

		check(b2network::crypto_scalarmult(&sharedKey[0], &privateKey[0], &ClientPublicKey[0]) == 0);

		::crypto_hash_sha512_init(&h);
		::crypto_hash_sha512_update(&h, &sharedKey[0], sizeof sharedKey);
		::crypto_hash_sha512_update(&h, &ClientPublicKey[0], sizeof ClientPublicKey);
		::crypto_hash_sha512_update(&h, &publicKey[0], sizeof publicKey);
		::crypto_hash_sha512_final(&h, &hashKey[0]);

		::memcpy(&realKey[0], &hashKey[0], crypto_stream_chacha20_KEYBYTES);
		::memcpy(&nonceKey[0], &hashKey[crypto_stream_chacha20_KEYBYTES], crypto_stream_chacha20_NONCEBYTES);
*/
	}

	std::string Key::GetPrivateKeyToString() const
	{
		//return hexify(&privateKey[0], crypto_stream_chacha20_KEYBYTES);
		return hexify(&privateKey[0], 32);
	}

	std::string Key::GetPublicKeyToString() const
	{
		//return hexify(&publicKey[0], crypto_stream_chacha20_KEYBYTES);
		return hexify(&publicKey[0], 32);
	}

/* comment out this code block to use later
	int32 Key::my_crypto_scalarmult_base(unsigned char *q, const unsigned char *n)
	{
		static const unsigned char basepoint[crypto_stream_chacha20_KEYBYTES] = {9};

		return b2network::crypto_scalarmult(q, n, basepoint);
	}
*/

	bool ChaCha20::Encrypt(unsigned char* body, uint32 size)
	{
		//if (body)
		//{
		//	::crypto_stream_chacha20_xor(body, body, size, &myKey.GetNonceKey()[0], &myKey.GetRealKey()[0]);
		//}

		return true;
	}

	bool ChaCha20::Decrypt(unsigned char* body, uint32 size)
	{
		//if (body)
		//{
		//	::crypto_stream_chacha20_xor(body, body, size, &myKey.GetNonceKey()[0], &myKey.GetRealKey()[0]);
		//}

		return true;
	}
}