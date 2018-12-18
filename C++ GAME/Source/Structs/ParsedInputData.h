#include <string>
using namespace std;

/*
 * Structure to hold input data once parsed to trigger actions with.
 */

struct ParsedInputData
{
public:
    /* Action (stored as JSON id) */
    void setActionID(int id) { action = id; }
    int getActionID() { return action; }

    /* Subject (stored as text matching JSON) */
    void setSubject(string subj) {subject = subj; }
    string getSubject() { return subject; }

    /* Subject validity (blank subject may be valid/invalid) */
    void setSubjectValidity(bool valid) { subject_valid = valid; }
    bool getSubjectValidity() { return subject_valid; }

    /* Reset */
    void clearAll() { action = 0; subject = ""; subject_valid = false; }

private:
    int action = 0;
    string subject = "";
    bool subject_valid = false;
};