
#ifndef TYPES_H_
#define TYPES_H_

    #ifndef SPACE_CHARACTER
        #define SPACE_CHARACTER ' '
    #endif
    
    #include <ctype.h>

    enum Copy_Mode_t
    {
        ToBUFFER=0,
        ToEEPROM
    };
    typedef enum Copy_Mode_t Copy_Mode;

    enum Menu_Dialog_t
    {
        Dialog_Result_None=0,
        Dialog_Result_Ok,
        Dialog_Result_Abort
    };
    typedef enum Menu_Dialog_t Menu_Dialog;

    struct Menu_Item_t
    {
        char acronym;
        const char *str;
    };
    typedef struct Menu_Item_t Menu_Item;

    unsigned char types_check_isnumeric(const char *s);

#endif /* TYPES_H_ */