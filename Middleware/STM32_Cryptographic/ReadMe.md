
1 - Download [X-CUBE-CRYPTOLIB ](https://www.st.com/en/embedded-software/x-cube-cryptolib.html) version package v4.5.0.

2 - Copy the content of "en.x-cube-cryptolib-v4-5-0.zip\STM32CubeExpansion_Crypto_V4.5.0\Middlewares\ST\STM32_Cryptographic" folder in this directory

Note : This directory must contain the following files and sub-directories:
- CMOX.chm
- LICENSE.txt
- ReadMe.txt
- Release_Notes.html
- _htmresc/
- include/
- interface/
- legacy_v3/
- lib/

3 - Comment following lines from "cmox_low_level_template.c" file . (RCC CRC management already handled at project's system.c level)

```
/**
  * @brief          CMOX library low level initialization
  * @param          pArg User defined parameter that is transmitted from initialize service
  * @retval         Initialization status: @ref CMOX_INIT_SUCCESS / @ref CMOX_INIT_FAIL
  */
cmox_init_retval_t cmox_ll_init(void *pArg)
{
  (void)pArg;
  /* Ensure CRC is enabled for cryptographic processing */
//  __HAL_RCC_CRC_RELEASE_RESET();
//  __HAL_RCC_CRC_CLK_ENABLE();
  return CMOX_INIT_SUCCESS;
}
```
