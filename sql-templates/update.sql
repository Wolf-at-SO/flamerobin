{%confgui%}update
  {%object_name%}
set {%foreach:column:, :{%ifcontains:{%getconf:{%object_path%}/columnNames%}:{%object_name%}:
  {%object_name%} = '{%object_name%}{%ifeq:{%columninfo:is_nullable%}:false:*%}'%}%}
where
  {%primary_key:{%constraintinfo:columns: and
  : = '?'%}%};