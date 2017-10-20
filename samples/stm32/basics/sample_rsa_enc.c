#include "basics.h"

#define KEY_LEN (2048/8)

#define RSA_N   "9EF0B9B7FD6E27FBA8DE3367BBABA5A4EB998E56940517C541E4115C3F197F2B70261DA3306601B185EAADE4FB43896D7774581EE7C47F982BDEB25A207A3486DAC681C9F1984A0F68B4A7A0977A621AC4C4964F53FF9BB22A74B6764039437508BDAB84FDD9A99E5034AA1FD5DFA6271C5792D8D8BF03175D99B368879E71EA1408CD2940F19905BD6AC5C1C8781CD10DE9C220B9FFD00B9D5598071F749188E81FD56509DDA6AC46F5CAB79B5D865B2AFE787EDB57D172D4347C59A7D2B3E6A90FD22B6088262F2C6F69BDCAC657A5ABF96A8507C42A2C593E2CA8D4BAD301D321566AE0354FAE9629FE471218A0E7835BCBCA6444802B5E0FC48C9D255B2F"
#define RSA_E   "010001"
#define RSA_D   "2E19C2B494449C424973D1D337C30CB3AE4686DBA3E96A40A3E1040C0E9D559E82D3FB6AB7020F61AF6727733052218D67C58E3DC099E52C7D2505F4505141E50F95DE5575C6E74C5AE17371881E4E50575C7200A66B907C78060C5A763C65CD8F707C4C81D8DD72F6EE9F9DC64A2D8650993215C5A079C4103921A9A3149913854FAD0E3B9AED5E7252632113915CD1BDCA5D26B8D2E6D4E8BD377E0CBF6777FB047776C5ACD03D62C644B8C026B208DDA1BC511D7C9795C73C79F45AEAD34E8C65EA6F51A805D0CD348320DB99A24D9DB00CBA4AEE43659BBB57820BB3433072BB9C2AC03CD69C1C680DB345217822332FCFB54C85EBC8E6D451DCDC750BB9"
#define RSA_P   "FB23A3A00A43307462CDD2DA2D0EEAA8C48B37383A1E3DA60511E49782FC822C1C5A3F1DBF7B5FE6284C785EDAD7357DCAFA838900DE08DFCA05FBBBC248E0B5C0BB8FB775367DF2F687832EA7EAD86C2682DFA6801B5A18FF7EF1752CED9629366AE4D4E8FC83AB93EC07F3EC41D85113ED99654168284C975D9AE04D23A5AD"
#define RSA_Q   "A20440D960251C1186C64D058C897FAFEF3EA3AC6531AAA3FB2AA97E575941E2FA4828D70675DBEBEF8EB41E7B11598E9BFA2E2873E631BFC0EDF6EA75C1DF7468DE49CA117E82A9EDAD2DBE0870FFB30CE882BAFB17FB21DDD934DA7C5339B2DD462E7CA0D5AA88601108A9B4570E5A4CA86084E3932BD5A315DD8A3C6647CB"
#define RSA_DP  "213C312E3FCD6E7F2888368C3B044B57B452FC26CBC22EC466CD5C6414A8980D805F0D1541BEF10D036E434BC24852C2E7F6C92B7B5AE6EFBECB95E9DE2748DDA4B8EDA708B3026E01E4231F2C87AEC6793E3E9CC93D556A137DD8347A92A299A7A2F3CF9D59B58F224F4D2A21AD011491BF92DA4C929D2AADA96C929C82C82D"
#define RSA_DQ  "4118AC65AAB7BE5B5C6778B2EE10A0F9338B5B7FFCF8C3B3374BA0E65E7A427ACC4D09FBBB60C78838AC81BA6D803F17899C2FC79929B87B6F2D2DB46EB402F066E2498BA313CDB431071005F6A65056C4ACF739E1817529DA55576321E8DE99FB46238F779FE1F10188568EB922F1655EBF98684C77E978E119FBF57AE72D99"
#define RSA_QP  "0DB6D20EB0A5E0481411D5AB90C9123DD3BF7D2E392198B69B46FCDF0A03DC0500FAE59617DDCC36A919B6B9F9CA0C7354B4112E50FCC1AF7B2038852B2057913A25AC9B98A5A3C6E58CDD8BBF4F489B512A6ACA0C626A66A907BFE73954144670D61575EB529740B132FADA9F318FA1883CF641C0D211B82B4CAD24CB59BA02"

#define PT_LEN  13
#define RSA_PT  "Hello, World!"

int myrand( void *rng_state, unsigned char *output, size_t len )
{
    if( rng_state != NULL )
        rng_state  = NULL;

    for( size_t i = 0; i < len; ++i )
        output[i] = rand();

    return( 0 );
}

int sample_rsa_enc(void)
{
    int ret = 0;
    size_t len;
    mbedtls_rsa_context rsa;
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;

    unsigned char rsa_plaintext[PT_LEN];
    unsigned char rsa_decrypted[PT_LEN];
    unsigned char rsa_ciphertext[KEY_LEN];
    const char *pers = "simple_rsa";
    
    mbedtls_rsa_init( &rsa, MBEDTLS_RSA_PKCS_V15, 0 );
    mbedtls_ctr_drbg_init( &ctr_drbg );
    mbedtls_entropy_init( &entropy );

    ret = mbedtls_ctr_drbg_seed( &ctr_drbg, mbedtls_entropy_func,
                                        &entropy, (const unsigned char *) pers,
                                        strlen( pers ) );
    if( ret != 0 )
    {
        mbedtls_printf( " failed\n  ! mbedtls_ctr_drbg_seed returned %d\n", ret );
        goto cleanup;
    }

    rsa.len = KEY_LEN;
    MBEDTLS_MPI_CHK( mbedtls_mpi_read_string( &rsa.N , 16, RSA_N  ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_read_string( &rsa.E , 16, RSA_E  ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_read_string( &rsa.D , 16, RSA_D  ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_read_string( &rsa.P , 16, RSA_P  ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_read_string( &rsa.Q , 16, RSA_Q  ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_read_string( &rsa.DP, 16, RSA_DP ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_read_string( &rsa.DQ, 16, RSA_DQ ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_read_string( &rsa.QP, 16, RSA_QP ) );

    printf( "  RSA key validation: " );

    if( mbedtls_rsa_check_pubkey(  &rsa ) != 0 ||
        mbedtls_rsa_check_privkey( &rsa ) != 0 )
    {
        printf( "failed\n" );
        goto cleanup;
    }
    else
    {
        printf( "success\n" );
    }

    printf( "PKCS#1 encryption : " );

    memcpy( rsa_plaintext, RSA_PT, PT_LEN );

    if( mbedtls_rsa_pkcs1_encrypt( &rsa, mbedtls_ctr_drbg_random, NULL, MBEDTLS_RSA_PUBLIC, PT_LEN,
                           rsa_plaintext, rsa_ciphertext ) != 0 )
    {
        printf( "failed\n" );
        goto cleanup;
    }
    else
    {
        printf( "success\n" );
    }


    printf( "PKCS#1 decryption : " );
    if( mbedtls_rsa_pkcs1_decrypt( &rsa, mbedtls_ctr_drbg_random, NULL, MBEDTLS_RSA_PRIVATE, &len,
                           rsa_ciphertext, rsa_decrypted,
                           sizeof(rsa_decrypted) ) != 0 )
    {
        printf( "failed\n" );
        goto cleanup;
    }

    if( memcmp( rsa_decrypted, rsa_plaintext, len ) != 0 )
    {
        printf( "failed\n" );
        goto cleanup;
    }

    printf( "success\n  Plaintext: %s\n", rsa_decrypted );

cleanup:
    mbedtls_entropy_free( &entropy );
    mbedtls_ctr_drbg_free( &ctr_drbg );
    mbedtls_rsa_free( &rsa );

    return ret;
}