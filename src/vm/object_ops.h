static inline ana_object *mul(ana_vm *vm, ana_object *a, ana_object *b)
{
  if(a->type->obj_binops != NULL && a->type->obj_binops->obj_mul != NULL) 
  {
    ana_object *res = a->type->obj_binops->obj_mul(a, b);
    if(res) 
    {
      GC_TRACK(vm, res);
      return res;
    }
  }
  return NULL;
}

static inline ana_object *sub(ana_vm *vm, ana_object *a, ana_object *b)
{
  if(a->type->obj_binops != NULL && a->type->obj_binops->obj_sub != NULL) 
  {
    ana_object *res = a->type->obj_binops->obj_sub(a, b);
    
    if(res) 
    {
      GC_TRACK(vm, res);
      
      return res;
    }
  }
  return NULL;
}

static inline ana_object *getindex(ana_vm *vm, ana_object *container, 
  ana_object *idx)
{
  if(container->type->obj_seqops != NULL && container->type->obj_seqops->get != NULL)
  {
    ana_object *res = container->type->obj_seqops->get(container, idx);

    if(!res)
      return ANA_KEY_NOT_FOUND;

    /* array values and map values, are already part of the GC root */
    if(!ana_type_is(container, ana_array_type) 
      && !ana_type_is(container, ana_map_type))
        GC_TRACK(vm, res);

    return res;
  }

  return NULL; 
}

static inline ana_object *setindex(ana_vm *vm, ana_object *container, 
  ana_object *idx, ana_object *val)
{
  if(container->type->obj_seqops != NULL && container->type->obj_seqops->set != NULL)
  {
    if(container->type->obj_seqops != NULL && container->type->obj_seqops->get != NULL)
    {
      ana_object *prev = container->type->obj_seqops->get(container, idx);;

      if(prev && prev->refcount > 0)
        prev->refcount--;
    }

    ana_object *res = container->type->obj_seqops->set(container, idx, val);

    if(!res)
      return ANA_KEY_NOT_FOUND;

    /* array values and map values, are already part of the GC root */
    if(!ana_type_is(container, ana_array_type) 
      && !ana_type_is(container, ana_map_type))
        GC_TRACK(vm, res);

    res->refcount++;

    return res;
  }

  return NULL; 
}

static inline ana_object *do_div(ana_vm *vm, ana_object *a, ana_object *b)
{
  ana_object *retval = NULL;

  if(a->type->obj_binops != NULL && a->type->obj_binops->obj_div != NULL) 
  {
    retval = a->type->obj_binops->obj_div(a, b);
    if(retval)
      GC_TRACK(vm, retval);
  }

  return retval;
}
