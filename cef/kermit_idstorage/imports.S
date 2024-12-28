	.set noreorder

#include "pspstub.s"
	STUB_START "sceWlanDrv_driver", 0x00090011, 0x000010005
	STUB_FUNC 0x19E51F54,sceWlanDevGetMacAddr
	STUB_END
	
	STUB_START "sceKermitPeripheral_driver", 0x00010011, 0x000010005
	STUB_FUNC 0xC6F8B4E1,sceKermitPeripheral_driver_0xC6F8B4E1
	STUB_END