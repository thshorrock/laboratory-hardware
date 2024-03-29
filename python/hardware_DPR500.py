# This file was automatically generated by SWIG (http://www.swig.org).
# Version 1.3.40
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.
# This file is compatible with both classic and new-style classes.

from sys import version_info
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_hardware_DPR500', [dirname(__file__)])
        except ImportError:
            import _hardware_DPR500
            return _hardware_DPR500
        if fp is not None:
            try:
                _mod = imp.load_module('_hardware_DPR500', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _hardware_DPR500 = swig_import_helper()
    del swig_import_helper
else:
    import _hardware_DPR500
del version_info
try:
    _swig_property = property
except NameError:
    pass # Python < 2.2 doesn't have 'property'.
def _swig_setattr_nondynamic(self,class_type,name,value,static=1):
    if (name == "thisown"): return self.this.own(value)
    if (name == "this"):
        if type(value).__name__ == 'SwigPyObject':
            self.__dict__[name] = value
            return
    method = class_type.__swig_setmethods__.get(name,None)
    if method: return method(self,value)
    if (not static) or hasattr(self,name):
        self.__dict__[name] = value
    else:
        raise AttributeError("You cannot add attributes to %s" % self)

def _swig_setattr(self,class_type,name,value):
    return _swig_setattr_nondynamic(self,class_type,name,value,0)

def _swig_getattr(self,class_type,name):
    if (name == "thisown"): return self.this.own()
    method = class_type.__swig_getmethods__.get(name,None)
    if method: return method(self)
    raise AttributeError(name)

def _swig_repr(self):
    try: strthis = "proxy of " + self.this.__repr__()
    except: strthis = ""
    return "<%s.%s; %s >" % (self.__class__.__module__, self.__class__.__name__, strthis,)

try:
    _object = object
    _newclass = 1
except AttributeError:
    class _object : pass
    _newclass = 0


class SwigPyIterator(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, SwigPyIterator, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, SwigPyIterator, name)
    def __init__(self, *args, **kwargs): raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    __swig_destroy__ = _hardware_DPR500.delete_SwigPyIterator
    __del__ = lambda self : None;
    def value(self): return _hardware_DPR500.SwigPyIterator_value(self)
    def incr(self, n = 1): return _hardware_DPR500.SwigPyIterator_incr(self, n)
    def decr(self, n = 1): return _hardware_DPR500.SwigPyIterator_decr(self, n)
    def distance(self, *args): return _hardware_DPR500.SwigPyIterator_distance(self, *args)
    def equal(self, *args): return _hardware_DPR500.SwigPyIterator_equal(self, *args)
    def copy(self): return _hardware_DPR500.SwigPyIterator_copy(self)
    def next(self): return _hardware_DPR500.SwigPyIterator_next(self)
    def __next__(self): return _hardware_DPR500.SwigPyIterator___next__(self)
    def previous(self): return _hardware_DPR500.SwigPyIterator_previous(self)
    def advance(self, *args): return _hardware_DPR500.SwigPyIterator_advance(self, *args)
    def __eq__(self, *args): return _hardware_DPR500.SwigPyIterator___eq__(self, *args)
    def __ne__(self, *args): return _hardware_DPR500.SwigPyIterator___ne__(self, *args)
    def __iadd__(self, *args): return _hardware_DPR500.SwigPyIterator___iadd__(self, *args)
    def __isub__(self, *args): return _hardware_DPR500.SwigPyIterator___isub__(self, *args)
    def __add__(self, *args): return _hardware_DPR500.SwigPyIterator___add__(self, *args)
    def __sub__(self, *args): return _hardware_DPR500.SwigPyIterator___sub__(self, *args)
    def __iter__(self): return self
SwigPyIterator_swigregister = _hardware_DPR500.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)

class floatArray(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, floatArray, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, floatArray, name)
    __repr__ = _swig_repr
    def __init__(self, *args): 
        this = _hardware_DPR500.new_floatArray(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _hardware_DPR500.delete_floatArray
    __del__ = lambda self : None;
    def __getitem__(self, *args): return _hardware_DPR500.floatArray___getitem__(self, *args)
    def __setitem__(self, *args): return _hardware_DPR500.floatArray___setitem__(self, *args)
    def cast(self): return _hardware_DPR500.floatArray_cast(self)
    __swig_getmethods__["frompointer"] = lambda x: _hardware_DPR500.floatArray_frompointer
    if _newclass:frompointer = staticmethod(_hardware_DPR500.floatArray_frompointer)
floatArray_swigregister = _hardware_DPR500.floatArray_swigregister
floatArray_swigregister(floatArray)

def floatArray_frompointer(*args):
  return _hardware_DPR500.floatArray_frompointer(*args)
floatArray_frompointer = _hardware_DPR500.floatArray_frompointer

class DPR500_exception(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, DPR500_exception, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, DPR500_exception, name)
    __repr__ = _swig_repr
    def __init__(self): 
        this = _hardware_DPR500.new_DPR500_exception()
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _hardware_DPR500.delete_DPR500_exception
    __del__ = lambda self : None;
DPR500_exception_swigregister = _hardware_DPR500.DPR500_exception_swigregister
DPR500_exception_swigregister(DPR500_exception)

class invalid_query(DPR500_exception):
    __swig_setmethods__ = {}
    for _s in [DPR500_exception]: __swig_setmethods__.update(getattr(_s,'__swig_setmethods__',{}))
    __setattr__ = lambda self, name, value: _swig_setattr(self, invalid_query, name, value)
    __swig_getmethods__ = {}
    for _s in [DPR500_exception]: __swig_getmethods__.update(getattr(_s,'__swig_getmethods__',{}))
    __getattr__ = lambda self, name: _swig_getattr(self, invalid_query, name)
    __repr__ = _swig_repr
    def __init__(self): 
        this = _hardware_DPR500.new_invalid_query()
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _hardware_DPR500.delete_invalid_query
    __del__ = lambda self : None;
invalid_query_swigregister = _hardware_DPR500.invalid_query_swigregister
invalid_query_swigregister(invalid_query)

class failed_to_contact_DPR500_device(DPR500_exception):
    __swig_setmethods__ = {}
    for _s in [DPR500_exception]: __swig_setmethods__.update(getattr(_s,'__swig_setmethods__',{}))
    __setattr__ = lambda self, name, value: _swig_setattr(self, failed_to_contact_DPR500_device, name, value)
    __swig_getmethods__ = {}
    for _s in [DPR500_exception]: __swig_getmethods__.update(getattr(_s,'__swig_getmethods__',{}))
    __getattr__ = lambda self, name: _swig_getattr(self, failed_to_contact_DPR500_device, name)
    __repr__ = _swig_repr
    def __init__(self): 
        this = _hardware_DPR500.new_failed_to_contact_DPR500_device()
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _hardware_DPR500.delete_failed_to_contact_DPR500_device
    __del__ = lambda self : None;
failed_to_contact_DPR500_device_swigregister = _hardware_DPR500.failed_to_contact_DPR500_device_swigregister
failed_to_contact_DPR500_device_swigregister(failed_to_contact_DPR500_device)

class pulser_receiver_not_attached_to_DPR500(DPR500_exception):
    __swig_setmethods__ = {}
    for _s in [DPR500_exception]: __swig_setmethods__.update(getattr(_s,'__swig_setmethods__',{}))
    __setattr__ = lambda self, name, value: _swig_setattr(self, pulser_receiver_not_attached_to_DPR500, name, value)
    __swig_getmethods__ = {}
    for _s in [DPR500_exception]: __swig_getmethods__.update(getattr(_s,'__swig_getmethods__',{}))
    __getattr__ = lambda self, name: _swig_getattr(self, pulser_receiver_not_attached_to_DPR500, name)
    __repr__ = _swig_repr
    def __init__(self): 
        this = _hardware_DPR500.new_pulser_receiver_not_attached_to_DPR500()
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _hardware_DPR500.delete_pulser_receiver_not_attached_to_DPR500
    __del__ = lambda self : None;
pulser_receiver_not_attached_to_DPR500_swigregister = _hardware_DPR500.pulser_receiver_not_attached_to_DPR500_swigregister
pulser_receiver_not_attached_to_DPR500_swigregister(pulser_receiver_not_attached_to_DPR500)

class incorrect_input_to_gain_command(DPR500_exception):
    __swig_setmethods__ = {}
    for _s in [DPR500_exception]: __swig_setmethods__.update(getattr(_s,'__swig_setmethods__',{}))
    __setattr__ = lambda self, name, value: _swig_setattr(self, incorrect_input_to_gain_command, name, value)
    __swig_getmethods__ = {}
    for _s in [DPR500_exception]: __swig_getmethods__.update(getattr(_s,'__swig_getmethods__',{}))
    __getattr__ = lambda self, name: _swig_getattr(self, incorrect_input_to_gain_command, name)
    __repr__ = _swig_repr
    def __init__(self): 
        this = _hardware_DPR500.new_incorrect_input_to_gain_command()
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _hardware_DPR500.delete_incorrect_input_to_gain_command
    __del__ = lambda self : None;
incorrect_input_to_gain_command_swigregister = _hardware_DPR500.incorrect_input_to_gain_command_swigregister
incorrect_input_to_gain_command_swigregister(incorrect_input_to_gain_command)

class requested_gain_out_of_bounds(DPR500_exception):
    __swig_setmethods__ = {}
    for _s in [DPR500_exception]: __swig_setmethods__.update(getattr(_s,'__swig_setmethods__',{}))
    __setattr__ = lambda self, name, value: _swig_setattr(self, requested_gain_out_of_bounds, name, value)
    __swig_getmethods__ = {}
    for _s in [DPR500_exception]: __swig_getmethods__.update(getattr(_s,'__swig_getmethods__',{}))
    __getattr__ = lambda self, name: _swig_getattr(self, requested_gain_out_of_bounds, name)
    __repr__ = _swig_repr
    def __init__(self): 
        this = _hardware_DPR500.new_requested_gain_out_of_bounds()
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _hardware_DPR500.delete_requested_gain_out_of_bounds
    __del__ = lambda self : None;
requested_gain_out_of_bounds_swigregister = _hardware_DPR500.requested_gain_out_of_bounds_swigregister
requested_gain_out_of_bounds_swigregister(requested_gain_out_of_bounds)

A = _hardware_DPR500.A
B = _hardware_DPR500.B
class damping_RPL2(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, damping_RPL2, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, damping_RPL2, name)
    __repr__ = _swig_repr
    threehundredthirty_Ohm = _hardware_DPR500.damping_RPL2_threehundredthirty_Ohm
    onehundredfour_Ohm = _hardware_DPR500.damping_RPL2_onehundredfour_Ohm
    fourtyfour_Ohm = _hardware_DPR500.damping_RPL2_fourtyfour_Ohm
    thirtyfour_Ohm = _hardware_DPR500.damping_RPL2_thirtyfour_Ohm
    def __init__(self): 
        this = _hardware_DPR500.new_damping_RPL2()
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _hardware_DPR500.delete_damping_RPL2
    __del__ = lambda self : None;
damping_RPL2_swigregister = _hardware_DPR500.damping_RPL2_swigregister
damping_RPL2_swigregister(damping_RPL2)

class damping_RPU2(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, damping_RPU2, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, damping_RPU2, name)
    __repr__ = _swig_repr
    onehundred_Ohm = _hardware_DPR500.damping_RPU2_onehundred_Ohm
    fifty_Ohm = _hardware_DPR500.damping_RPU2_fifty_Ohm
    twentyone_Ohm = _hardware_DPR500.damping_RPU2_twentyone_Ohm
    seventeen_Ohm = _hardware_DPR500.damping_RPU2_seventeen_Ohm
    def __init__(self): 
        this = _hardware_DPR500.new_damping_RPU2()
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _hardware_DPR500.delete_damping_RPU2
    __del__ = lambda self : None;
damping_RPU2_swigregister = _hardware_DPR500.damping_RPU2_swigregister
damping_RPU2_swigregister(damping_RPU2)

class energy_policy(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, energy_policy, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, energy_policy, name)
    __repr__ = _swig_repr
    LOW = _hardware_DPR500.energy_policy_LOW
    HIGH = _hardware_DPR500.energy_policy_HIGH
    def __init__(self): 
        this = _hardware_DPR500.new_energy_policy()
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _hardware_DPR500.delete_energy_policy
    __del__ = lambda self : None;
energy_policy_swigregister = _hardware_DPR500.energy_policy_swigregister
energy_policy_swigregister(energy_policy)

class receive_mode(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, receive_mode, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, receive_mode, name)
    __repr__ = _swig_repr
    PULSE_ECHO = _hardware_DPR500.receive_mode_PULSE_ECHO
    RECEIVE_ONLY = _hardware_DPR500.receive_mode_RECEIVE_ONLY
    def __init__(self): 
        this = _hardware_DPR500.new_receive_mode()
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _hardware_DPR500.delete_receive_mode
    __del__ = lambda self : None;
receive_mode_swigregister = _hardware_DPR500.receive_mode_swigregister
receive_mode_swigregister(receive_mode)

class HPFA(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, HPFA, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, HPFA, name)
    __repr__ = _swig_repr
    ten_KHz = _hardware_DPR500.HPFA_ten_KHz
    one_MHz = _hardware_DPR500.HPFA_one_MHz
    twopointfive_MHz = _hardware_DPR500.HPFA_twopointfive_MHz
    five_MHz = _hardware_DPR500.HPFA_five_MHz
    sevenpointfive_MHz = _hardware_DPR500.HPFA_sevenpointfive_MHz
    def __init__(self): 
        this = _hardware_DPR500.new_HPFA()
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _hardware_DPR500.delete_HPFA
    __del__ = lambda self : None;
HPFA_swigregister = _hardware_DPR500.HPFA_swigregister
HPFA_swigregister(HPFA)

class LPFA(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, LPFA, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, LPFA, name)
    __repr__ = _swig_repr
    three_MHz = _hardware_DPR500.LPFA_three_MHz
    sevenpointfive_MHz = _hardware_DPR500.LPFA_sevenpointfive_MHz
    ten_MHz = _hardware_DPR500.LPFA_ten_MHz
    fifteen_MHz = _hardware_DPR500.LPFA_fifteen_MHz
    twentytwopointfive_MHz = _hardware_DPR500.LPFA_twentytwopointfive_MHz
    fifty_MHz = _hardware_DPR500.LPFA_fifty_MHz
    def __init__(self): 
        this = _hardware_DPR500.new_LPFA()
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _hardware_DPR500.delete_LPFA
    __del__ = lambda self : None;
LPFA_swigregister = _hardware_DPR500.LPFA_swigregister
LPFA_swigregister(LPFA)

class HPFB(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, HPFB, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, HPFB, name)
    __repr__ = _swig_repr
    five_MHz = _hardware_DPR500.HPFB_five_MHz
    thirty_MHz = _hardware_DPR500.HPFB_thirty_MHz
    def __init__(self): 
        this = _hardware_DPR500.new_HPFB()
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _hardware_DPR500.delete_HPFB
    __del__ = lambda self : None;
HPFB_swigregister = _hardware_DPR500.HPFB_swigregister
HPFB_swigregister(HPFB)

class LPFB(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, LPFB, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, LPFB, name)
    __repr__ = _swig_repr
    fivehundred_MHz = _hardware_DPR500.LPFB_fivehundred_MHz
    threehundred_MHz = _hardware_DPR500.LPFB_threehundred_MHz
    def __init__(self): 
        this = _hardware_DPR500.new_LPFB()
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _hardware_DPR500.delete_LPFB
    __del__ = lambda self : None;
LPFB_swigregister = _hardware_DPR500.LPFB_swigregister
LPFB_swigregister(LPFB)

class pulser_receiver(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, pulser_receiver, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, pulser_receiver, name)
    def __init__(self, *args, **kwargs): raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    def get_channel_char(self): return _hardware_DPR500.pulser_receiver_get_channel_char(self)
    def get_address_char(self): return _hardware_DPR500.pulser_receiver_get_address_char(self)
    def get_gain(self): return _hardware_DPR500.pulser_receiver_get_gain(self)
    def get_serial_number(self): return _hardware_DPR500.pulser_receiver_get_serial_number(self)
    __swig_destroy__ = _hardware_DPR500.delete_pulser_receiver
    __del__ = lambda self : None;
pulser_receiver_swigregister = _hardware_DPR500.pulser_receiver_swigregister
pulser_receiver_swigregister(pulser_receiver)

class RPL2(pulser_receiver):
    __swig_setmethods__ = {}
    for _s in [pulser_receiver]: __swig_setmethods__.update(getattr(_s,'__swig_setmethods__',{}))
    __setattr__ = lambda self, name, value: _swig_setattr(self, RPL2, name, value)
    __swig_getmethods__ = {}
    for _s in [pulser_receiver]: __swig_getmethods__.update(getattr(_s,'__swig_getmethods__',{}))
    __getattr__ = lambda self, name: _swig_getattr(self, RPL2, name)
    def __init__(self, *args, **kwargs): raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    def set_pulser_mode(self, *args): return _hardware_DPR500.RPL2_set_pulser_mode(self, *args)
    def set_prf(self, *args): return _hardware_DPR500.RPL2_set_prf(self, *args)
    def set_internal_trigger(self): return _hardware_DPR500.RPL2_set_internal_trigger(self)
    def set_external_trigger(self): return _hardware_DPR500.RPL2_set_external_trigger(self)
    def set_master(self): return _hardware_DPR500.RPL2_set_master(self)
    def set_slave(self): return _hardware_DPR500.RPL2_set_slave(self)
    def set_voltage(self, *args): return _hardware_DPR500.RPL2_set_voltage(self, *args)
    __swig_destroy__ = _hardware_DPR500.delete_RPL2
    __del__ = lambda self : None;
RPL2_swigregister = _hardware_DPR500.RPL2_swigregister
RPL2_swigregister(RPL2)

class RPU2(pulser_receiver):
    __swig_setmethods__ = {}
    for _s in [pulser_receiver]: __swig_setmethods__.update(getattr(_s,'__swig_setmethods__',{}))
    __setattr__ = lambda self, name, value: _swig_setattr(self, RPU2, name, value)
    __swig_getmethods__ = {}
    for _s in [pulser_receiver]: __swig_getmethods__.update(getattr(_s,'__swig_getmethods__',{}))
    __getattr__ = lambda self, name: _swig_getattr(self, RPU2, name)
    def __init__(self, *args, **kwargs): raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    def set_pulser_mode(self, *args): return _hardware_DPR500.RPU2_set_pulser_mode(self, *args)
    def set_prf(self, *args): return _hardware_DPR500.RPU2_set_prf(self, *args)
    def set_internal_trigger(self): return _hardware_DPR500.RPU2_set_internal_trigger(self)
    def set_external_trigger(self): return _hardware_DPR500.RPU2_set_external_trigger(self)
    def set_master(self): return _hardware_DPR500.RPU2_set_master(self)
    def set_slave(self): return _hardware_DPR500.RPU2_set_slave(self)
    __swig_destroy__ = _hardware_DPR500.delete_RPU2
    __del__ = lambda self : None;
RPU2_swigregister = _hardware_DPR500.RPU2_swigregister
RPU2_swigregister(RPU2)

class PulserReceiverFactory(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, PulserReceiverFactory, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, PulserReceiverFactory, name)
    __repr__ = _swig_repr
    def make_RPL2(self): return _hardware_DPR500.PulserReceiverFactory_make_RPL2(self)
    def make_RPU2(self): return _hardware_DPR500.PulserReceiverFactory_make_RPU2(self)
    def __init__(self): 
        this = _hardware_DPR500.new_PulserReceiverFactory()
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _hardware_DPR500.delete_PulserReceiverFactory
    __del__ = lambda self : None;
PulserReceiverFactory_swigregister = _hardware_DPR500.PulserReceiverFactory_swigregister
PulserReceiverFactory_swigregister(PulserReceiverFactory)

class DPR500(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, DPR500, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, DPR500, name)
    __repr__ = _swig_repr
    def __init__(self, *args): 
        this = _hardware_DPR500.new_DPR500(*args)
        try: self.this.append(this)
        except: self.this = this
    def notify(self): return _hardware_DPR500.DPR500_notify(self)
    def blink(self, speed = 100): return _hardware_DPR500.DPR500_blink(self, speed)
    def attach_A(self, *args): return _hardware_DPR500.DPR500_attach_A(self, *args)
    def attach_B(self, *args): return _hardware_DPR500.DPR500_attach_B(self, *args)
    def detach_A(self): return _hardware_DPR500.DPR500_detach_A(self)
    def detach_B(self): return _hardware_DPR500.DPR500_detach_B(self)
    def turn_on_A(self): return _hardware_DPR500.DPR500_turn_on_A(self)
    def turn_off_A(self): return _hardware_DPR500.DPR500_turn_off_A(self)
    def turn_on_B(self): return _hardware_DPR500.DPR500_turn_on_B(self)
    def turn_off_B(self): return _hardware_DPR500.DPR500_turn_off_B(self)
    def set_gain_A(self, *args): return _hardware_DPR500.DPR500_set_gain_A(self, *args)
    def set_high_pass_filter_A(self, *args): return _hardware_DPR500.DPR500_set_high_pass_filter_A(self, *args)
    def set_low_pass_filter_A(self, *args): return _hardware_DPR500.DPR500_set_low_pass_filter_A(self, *args)
    def set_gain_B(self, *args): return _hardware_DPR500.DPR500_set_gain_B(self, *args)
    def set_high_pass_filter_B(self, *args): return _hardware_DPR500.DPR500_set_high_pass_filter_B(self, *args)
    def set_low_pass_filter_B(self, *args): return _hardware_DPR500.DPR500_set_low_pass_filter_B(self, *args)
    __swig_destroy__ = _hardware_DPR500.delete_DPR500
    __del__ = lambda self : None;
DPR500_swigregister = _hardware_DPR500.DPR500_swigregister
DPR500_swigregister(DPR500)



