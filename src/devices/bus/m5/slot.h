// license:BSD-3-Clause
// copyright-holders:Fabio Priuli
#ifndef __M5_SLOT_H
#define __M5_SLOT_H

/***************************************************************************
 TYPE DEFINITIONS
 ***************************************************************************/


/* PCB */
enum
{
	EM_5 = 1,
	MEM64KBI,
	MEM64KBF,
	MEM64KRX
};


#define M5_STD  0


// ======================> device_m5_cart_interface

class device_m5_cart_interface : public device_slot_card_interface
{
public:
	// construction/destruction
	device_m5_cart_interface(const machine_config &mconfig, device_t &device);
	virtual ~device_m5_cart_interface();

	// reading and writing
	virtual DECLARE_READ8_MEMBER(read_rom) { return 0xff; }
	virtual DECLARE_READ8_MEMBER(read_ram) { return 0xff; }
	virtual DECLARE_WRITE8_MEMBER(write_ram) {}

	void rom_alloc(UINT32 size, const char *tag);
	void ram_alloc(UINT32 size);
	UINT8* get_rom_base() { return m_rom; }
	UINT8* get_ram_base() { return &m_ram[0]; }
	UINT32 get_rom_size() { return m_rom_size; }
	UINT32 get_ram_size() { return m_ram.size(); }

	void save_ram() { device().save_item(NAME(m_ram)); }

protected:
	// internal state
	UINT8 *m_rom;
	UINT32 m_rom_size;
	dynamic_buffer m_ram;
};


// ======================> m5_cart_slot_device

class m5_cart_slot_device : public device_t,
								public device_image_interface,
								public device_slot_interface
{
public:
	// construction/destruction
	m5_cart_slot_device(const machine_config &mconfig, const char *tag, device_t *owner, UINT32 clock);
	virtual ~m5_cart_slot_device();

	// device-level overrides
	virtual void device_start() override;
	virtual void device_config_complete() override;

	// image-level overrides
	virtual bool call_load() override;
	virtual void call_unload() override {}
	virtual const software_list_loader &get_software_list_loader() const override { return rom_software_list_loader::instance(); }

	int get_type() { return m_type; }

	void save_ram() { if (m_cart && m_cart->get_ram_size()) m_cart->save_ram(); }

	virtual iodevice_t image_type() const override { return IO_CARTSLOT; }
	virtual bool is_readable()  const override { return 1; }
	virtual bool is_writeable() const override { return 0; }
	virtual bool is_creatable() const override { return 0; }
	virtual bool must_be_loaded() const override { return 0; }
	virtual bool is_reset_on_load() const override { return 1; }
	virtual const char *image_interface() const override { return "m5_cart"; }
	virtual const char *file_extensions() const override { return "bin,rom"; }

	// slot interface overrides
	virtual std::string get_default_card_software() override;

	// reading and writing
	virtual DECLARE_READ8_MEMBER(read_rom);
	virtual DECLARE_READ8_MEMBER(read_ram);
	virtual DECLARE_WRITE8_MEMBER(write_ram);

protected:

	int m_type;
	device_m5_cart_interface*       m_cart;
};



// device type definition
extern const device_type M5_CART_SLOT;


/***************************************************************************
 DEVICE CONFIGURATION MACROS
 ***************************************************************************/

#define M5SLOT_ROM_REGION_TAG ":cart:rom"

#define MCFG_M5_CARTRIDGE_ADD(_tag,_slot_intf,_def_slot) \
	MCFG_DEVICE_ADD(_tag, M5_CART_SLOT, 0) \
	MCFG_DEVICE_SLOT_INTERFACE(_slot_intf, _def_slot, false)
#endif
