/*
*   file:   hal_sim.c
*
*   Mocks HAL functions with debug versions. Allows testing without
*   a physical ARM board.
*
*/

// UART functions
HAL_StatusTypeDef HAL_UART_Transmit(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout) {
    printf("uart transmit on handler: %p, data: %s, size: %i, timeout: %i\n",
            huart, pData, Size, Timeout);
    return HAL_OK;
}

HAL_StatusTypeDef HAL_UART_Receive(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout) {
    printf("uart receive on handler: %p, data: %s, size: %i, timeout: %i\n",
            huart, pData, Size, Timeout);
    return HAL_OK;
}


// I2C functions
HAL_StatusTypeDef HAL_I2C_Mem_Write(I2C_HandleTypeDef *hi2c, uint16_t DevAddress,
                            uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData,
                            uint16_t Size, uint32_t Timeout) {
    printf("i2c write on handler: %p, dev addr: %02x, mem addres: %02x, mem addr. size: %i,
            %s data, size: %i, timeout: %i\n", hi2c, DevAddress, MemAddress, MemaddSize
            pData, Size, Timeout);
    return HAL_OK;
}

HAL_StatusTypeDef HAL_I2C_Mem_Read(I2C_HandleTypeDef *hi2c, uint16_t DevAddress,
                                   uint16_t MemAddress, uint16_t MemAddSize,
                                   uint8_t *pData, uint16_t Size, uint32_t Timeout) {
    printf("i2c write on handler: %p, dev addr: %02x, mem addres: %02x, mem addr. size: %i,
           %s data, size: %i, timeout: %i\n", hi2c, DevAddress, MemAddress, MemaddSize
           pData, Size, Timeout);
    return HAL_OK;
}
