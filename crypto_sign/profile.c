#include "api.h"
#include "hal.h"
#include "hal-tracetx.h"
#include "sendfn.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define MLEN 59

// https://stackoverflow.com/a/1489985/1711232
#define PASTER(x, y) x##y
#define EVALUATOR(x, y) PASTER(x, y)
#define NAMESPACE(fun) EVALUATOR(MUPQ_NAMESPACE, fun)

// use different names so we can have empty namespaces
#define MUPQ_CRYPTO_PUBLICKEYBYTES NAMESPACE(CRYPTO_PUBLICKEYBYTES)
#define MUPQ_CRYPTO_SECRETKEYBYTES NAMESPACE(CRYPTO_SECRETKEYBYTES)
#define MUPQ_CRYPTO_BYTES          NAMESPACE(CRYPTO_BYTES)
#define MUPQ_CRYPTO_ALGNAME        NAMESPACE(CRYPTO_ALGNAME)

#define MUPQ_crypto_sign_keypair NAMESPACE(crypto_sign_keypair)
#define MUPQ_crypto_sign NAMESPACE(crypto_sign)
#define MUPQ_crypto_sign_open NAMESPACE(crypto_sign_open)
#define MUPQ_crypto_sign_signature NAMESPACE(crypto_sign_signature)
#define MUPQ_crypto_sign_verify NAMESPACE(crypto_sign_verify)

#define printcycles(S, U) send_unsignedll((S), (U))

/* Globals */
uint8_t txbuffer0[256]="Set the peripheral to control DMA flow. Useful w\n";
uint8_t txbuffer1[256]="Reset all stream interrupt flags using the inter\n";

int main(void)
{
  unsigned char sk[MUPQ_CRYPTO_SECRETKEYBYTES];
  unsigned char pk[MUPQ_CRYPTO_PUBLICKEYBYTES];
  unsigned char sm[MLEN+MUPQ_CRYPTO_BYTES];
  size_t smlen;
  int i;

  hal_setup(CLOCK_BENCHMARK);

  hal_send_str("==========================");

	tracetx_dma_setup();
	tracetx_set_lenght(49);

	for (i = 0; i < 10; i++) {

		tracetx_start(txbuffer0);

		// busy wait for DMA transfer complete
		tracetx_wait();

		tracetx_start(txbuffer1);

		// busy wait for DMA transfer complete
		tracetx_wait();
	}


	while (1);

  for(i=0;i<MUPQ_ITERATIONS; i++)
  {
    // Key-pair generation
    MUPQ_crypto_sign_keypair(pk, sk);
    hal_send_str("keypair cycles:");

    // Signing
    MUPQ_crypto_sign(sm, &smlen, sm, MLEN, sk);
    hal_send_str("sign cycles:");

    // Verification
    MUPQ_crypto_sign_open(sm, &smlen, sm, smlen, pk);
    hal_send_str("verify cycles:");

    hal_send_str("+");
  }
  hal_send_str("#");
  return 0;
}
