
#ifndef H_infra_util_Type
#define H_infra_util_Type

class Type {
   public:
   /// Different literal types
   enum Type_ID {
      Type_URI, Type_Literal, Type_CustomLanguage, Type_CustomType,
      Type_String, Type_Integer, Type_Decimal, Type_Double, Type_Boolean,Type_Date,Type_Time
   };
   /// Does the type have a sub-type?
   static inline bool hasSubType(Type_ID t) { return (t==Type_CustomLanguage)||(t==Type_CustomType); }
   /// Get the type of the sub-type
   static inline Type_ID getSubTypeType(Type_ID t) { return (t==Type_CustomLanguage)?Type_Literal:Type_URI; }
};
//---------------------------------------------------------------------------
#endif
